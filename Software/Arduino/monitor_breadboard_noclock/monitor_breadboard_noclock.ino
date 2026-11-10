/*
    Monitor buses on the breadboard, using the Arduino for clock pulses.

    Print in the serial monitor the A0-A15 address and D0-D7 data values of a W65C02S microprocessor at each clock cycle. An additional 8-bit data can be monitored
    The data line outputs its value in binary, hexadecimal, and OpCode. A marker is shown when either the RESET vector or a double NOP instruction is reached.
    The Arduino can stop the clock automatically when a double NOP instruction is reached, and it can resume when a button is pressed.
*/



// Operation parameters
const bool halt = true;  // Halt the clock module if NOP appears twice in a row on 'data1'   
const bool data2_flag = false;  // Display the second data stream


// Pins parameters
const int clock_pin = 2;  // Arduino pin with the clock signal
const int rw_pin = 3;  // Arduino pin connected to the W65C02S read/write pin
const int resume_step_pin = 4;  // Arduino pin to resume the automatic clock or to step the manual clock
const int select_pin = 5;  // Arduino pin to select the clock
const int addr_pins[] = {52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22};  // Arduino pins connected to W65C02S A0-A15, in reverse order
const int data_pins1[] = {53, 51, 49, 47, 45, 43, 41, 39};  // Arduino pins connected to W65C02S D0-D7, in reverse order
const int data_pins2[] = {37, 35, 33, 31, 29, 27, 25, 23};  // Arduino pins connected to a second data set, in reverse order


// Instructions arrays
const int instr_opc[] = {0x00, 0x01, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0C, 0x0D, 0x0E, 0x0F,
                         0x10, 0x11, 0x12, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1C, 0x1D, 0x1E, 0x1F,
                         0x20, 0x21, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2C, 0x2D, 0x2E, 0x2F,
                         0x30, 0x31, 0x32, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3C, 0x3D, 0x3E, 0x3F,
                         0x40, 0x41, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4C, 0x4D, 0x4E, 0x4F,
                         0x50, 0x51, 0x52, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5D, 0x5E, 0x5F,
                         0x60, 0x61, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6C, 0x6D, 0x6E, 0x6F,
                         0x70, 0x71, 0x72, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7C, 0x7D, 0x7E, 0x7F,
                         0x80, 0x81, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8C, 0x8D, 0x8E, 0x8F,
                         0x90, 0x91, 0x92, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9C, 0x9D, 0x9E, 0x9F,
                         0xA0, 0xA1, 0xA2, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAC, 0xAD, 0xAE, 0xAF,
                         0xB0, 0xB1, 0xB2, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBC, 0xBD, 0xBE, 0xBF,
                         0xC0, 0xC1, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
                         0xD0, 0xD1, 0xD2, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDD, 0xDE, 0xDF,
                         0xE0, 0xE1, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEC, 0xED, 0xEE, 0xEF,
                         0xF0, 0xF1, 0xF2, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFD, 0xFE, 0xFF};  // Instruction OpCodes
const char* instr_name[] = {"BRK", "ORA", "TSB", "ORA", "ASL", "RMB0", "PHP", "ORA", "ASL", "TSB", "ORA", "ASL", "BBR0",
                            "BPL", "ORA", "ORA", "TRB", "ORA", "ASL", "RMB1", "CLC", "ORA", "INC", "TRB", "ORA", "ASL", "BBR1",
                            "JSR", "AND", "BIT", "AND", "ROL", "RMB2", "PLP", "AND", "ROL", "BIT", "AND", "ROL", "BBR2",
                            "BMI", "AND", "AND", "BIT", "AND", "ROL", "RMB3", "SEC", "AND", "DEC", "BIT", "AND", "ROL", "BBR3",
                            "RTI", "EOR", "EOR", "LSR", "RMB4", "PHA", "EOR", "LSR", "JMP", "EOR", "LSR", "BBR4",
                            "BVC", "EOR", "EOR", "EOR", "LSR", "RMB5", "CLI", "EOR", "PHY", "EOR", "LSR", "BBR5",
                            "RTS", "ADC", "STZ", "ADC", "ROR", "RMB6", "PLA", "ADC", "ROR", "JMP", "ADC", "ROR", "BBR6",
                            "BVS", "ADC", "ADC", "STZ", "ADC", "ROR", "RMB7", "SEI", "ADC", "PLY", "JMP", "ADC", "ROR", "BBR7",
                            "BRA", "STA", "STY", "STA", "STZ", "SMB0", "DEY", "BIT", "TXA", "STY", "STA", "STX", "BBS0",
                            "BCC", "STA", "STA", "STY", "STA", "STZ", "SMB1", "TYA", "STA", "TXS", "STZ", "STA", "STZ", "BBS1",
                            "LDY", "LDA", "LDX", "LDY", "LDA", "LDX", "SMB2", "TAY", "LDA", "TAX", "LDY", "LDA", "LDX", "BBS2",
                            "BCS", "LDA", "LDA", "LDY", "LDA", "LDX", "SMB3", "CLV", "LDA", "TSX", "LDY", "LDA", "LDX", "BBS3",
                            "CPY", "CMP", "CPY", "CMP", "DEC", "SMB4", "INY", "CMP", "DEX", "WAI", "CPY", "CMP", "DEC", "BBS4",
                            "BNE", "CMP", "CMP", "CMP", "DEC", "SMB5", "CLD", "CMP", "PHX", "STP", "CMP", "DEC", "BBS5",
                            "CPX", "SBC", "CPX", "SBC", "INC", "SMB6", "INX", "SBC", "NOP", "CPX", "SBC", "INC", "BBS6",
                            "BEQ", "SBC", "SBC", "SBC", "INC", "SMB7", "SED", "SBC", "PLX", "SBC", "INC", "BBS7"};  // Instruction names


// Variables
int addr_prev, bit, count_eq, count_n, data1_prev, data2, instr_len, n_eq, n_n;
bool instr_flag1, instr_flag2;
char buffer[10], hex[40], line[15], opcod1[5], opcod2[5], rw;

int addr = 0, data1 = 0, line_n = 0, nop_acc = 0;
bool nop = false;



// Setup
void setup() {


  // Serial port
  Serial.begin (9600);  // Set speed
  while (!Serial) {}  // Wait for the serial port
  clear();


  // Clock pin
  pinMode(clock_pin, OUTPUT);  // Set as output


  // Read/write pin
  pinMode(rw_pin, INPUT);  // Set as input


  // Resume/Step pin
  pinMode(resume_step_pin, INPUT);  // Set as input


  // Select pin
  pinMode(select_pin, INPUT);  // Set as input
  

  // Address pins
  for (int i = 0; i < 16; i++) {
    pinMode(addr_pins[i], INPUT);  // Set as input
  }


  // Data pins
  for (int i = 0; i < 8; i++) {
    pinMode(data_pins1[i], INPUT);  // Set as input
    pinMode(data_pins2[i], INPUT);  // Set as input
  }
}



// Loop
void loop() {


  // Automatic clock
  if (digitalRead(select_pin) && !nop) {
    onClock();

    // Check NOP
    if (nop_acc > 2) {
      nop = true;
      nop_acc = 0;
    } else {
      nop = false;
    }
  }


  // Resume automatic clock
  if (digitalRead(select_pin) && !digitalRead(resume_step_pin) && nop) {
    nop = false;
    delay(300);  // Debouncer
  }


  // Manual clock
  if (!digitalRead(select_pin) && !digitalRead(resume_step_pin)) {  
    onClock();
    nop = false;
    delay(300);  // Debouncer
  }  
}



// Clock
void onClock() {


  // Tick
  digitalWrite(clock_pin, HIGH);
  delay(1);


  // Monitor address pins
  addr_prev = addr;

  for (int i = 0; i < 16; i++) {
    bit = (digitalRead(addr_pins[i]) ? 1 : 0);  // Read the pin bit
    addr = (addr << 1) + bit;  // Add the bit to the address
  }


  // Check reset
  if (addr == 0xFFFC) {  // Reset vector address
    clear();
    line_n = 0;
  }


  // Line number
  line_n++;
  memset(line, 0, sizeof(line));
  sprintf(line, " %7d      ", line_n);
  Serial.print(line);


  // Monitor read/write pin
  rw = (digitalRead(rw_pin) ? 'R' : 'W');
  Serial.print(rw);


  // Monitor address pins
  Serial.print("      0b");

  for (int i = 0; i < 16; i++) {
    bit = (digitalRead(addr_pins[i]) ? 1 : 0);  // Read the pin bit
    Serial.print(bit);
    addr = (addr << 1) + bit;  // Add the bit to the address
  }


  // Monitor data pins
  data1_prev = data1;
  data1 = 0;
  data2 = 0;
  Serial.print("   0b");

  for (int i = 0; i < 8; i++) {
    bit = (digitalRead(data_pins1[i]) ? 1 : 0);  // Read the pin bit
    Serial.print(bit);
    data1 = (data1 << 1) + bit;  // Add the bit to the data
  }

  Serial.print("   ");

  if (data2_flag == true) {

    Serial.print("   0b");

    for (int i = 0; i < 8; i++) {
      bit = (digitalRead(data_pins2[i]) ? 1 : 0);  // Read the pin bit
      Serial.print(bit);
      data2 = (data2 << 1) + bit;  // Add the bit to the data
    }

    Serial.print("   ");
  }


  // Tock
  digitalWrite(clock_pin, LOW);
  delay(1);


  // Print hexadecimal values
  memset(hex, 0, sizeof(hex));

  if (data2_flag == true) {
    sprintf(hex, "   0x%04X   0x%02X   0x%02X", addr, data1, data2);
  } else {
    sprintf(hex, "   0x%04X   0x%02X", addr, data1);
  }

  Serial.print(hex);


  // Print instruction
  instr_len = sizeof(instr_opc) / sizeof(int);
  instr_flag1 = false;
  instr_flag2 = false;
  Serial.print("      ");

  for (int i = 0; i < instr_len; i++) {

    if (data1 == instr_opc[i]) {
      instr_flag1 = true;
      memset(opcod1, 0, sizeof(opcod1));
      sprintf(opcod1, "%4s", instr_name[i]);
      Serial.print(opcod1);
    }
  }

  if (not instr_flag1) {
    Serial.print("----");
  }

  if (data2_flag == true) {
    Serial.print("    ");

    for (int i = 0; i < instr_len; i++) {

      if (data2 == instr_opc[i]) {
        instr_flag2 = true;
        memset(opcod2, 0, sizeof(opcod2));
        sprintf(opcod2, "%4s", instr_name[i]);
        Serial.print(opcod2);
      }
    }

    if (not instr_flag2) {
      Serial.print("----");
    }
  }


  // Print bookmark
  if (data1 == 0xEA) {  // Check for NOP
    nop_acc++;  // Increase NOP counter

    if (nop_acc > 2) {
      Serial.print("   ---> BOOKMARK");
    }
  } else {
    nop_acc = 0;  // Clear NOP counter
  }


  // Print reset
  if (addr == 0xFFFC) {  // Reset vector address
    Serial.print("   ---> RESET");
  }

  Serial.print("\n");
}



// Clear the monitor
void clear() {

  // Clear
  count_n = 0;
  count_eq = 0;
  n_n = 20;
  n_eq = 120;

  while (count_n ++ < n_n) {
    Serial.print("\n");
  }

  while (count_eq ++ < n_eq) {
    Serial.print("=");
  }

  Serial.println();
  Serial.println();
}
