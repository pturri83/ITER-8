/*
    Monitor address and data pins

    Print in the serial monitor the A0-A15 address and D0-D7 data values of a W65C02S microprocessor at each clock cycle.
    The data line outputs its value in binary, hexadecimal, and OpCode. A marker is shown when either the RESET vector or a NOP instruction is hit.
    The clock module can be stopped automatically when a NOP instruction is reached.

    Created on 2023-09-14
    By Paolo Turri
*/

// Settings parameters
const bool HALT = true;   // Halt the clock module if NOP is detected

// Pins parameters
const int CLOCK_PIN = 2;    // Arduino pin connected to the clock signal
const int RW_PIN = 3;    // Arduino pin connected to the W65C02S read/write pin
const int HALT_PIN = 4;    // Arduino pin connected to the clock module's halt pin
const int ADDR_PINS[] = {52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22};   // Arduino pins connected to W65C02S A0-A15, in reverse orderOpCodes
const int DATA_PINS1[] = {53, 51, 49, 47, 45, 43, 41, 39};   // Arduino pins connected to W65C02S D0-D7, in reverse order
const int DATA_PINS2[] = {37, 35, 33, 31, 29, 27, 25, 23};   // Arduino pins connected to a second data set, in reverse order

// Instructions arrays
const int INSTR_OPC[] = {0x00, 0x01, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0C, 0x0D, 0x0E, 0x0F,
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
const char* INSTR_NAME[] = {"BRK", "ORA", "TSB", "ORA", "ASL", "RMB0", "PHP", "ORA", "ASL", "TSB", "ORA", "ASL", "BBR0",
                            "BPL", "ORA", "ORA", "TRB", "ORA", "ASL", "RMB1", "CLC", "ORA", "INC", "TRB", "ORA", "ASL", "BBR1",
                            "JSR", "AND", "BIT", "AND", "ROL", "RMB2", "PLP", "AND", "ROL", "BIT", "AND", "ROL", "BBR2",
                            "BMI", "AND", "AND", "BIT", "AND", "ROL", "RMB3", "SEC", "AND", "DEC", "BIT", "AND", "ROL", "BBR3",
                            "RTI", "EOR", "EOR", "LSR", "RMB4", "PHA", "EOR", "LSR", "JMP", "EOR", "LSR", "BBR4",
                            "BVC", "EOR", "EOR", "EOR", "LSR", "RMB5", "CLI", "EOR", "PHY", "EOR", "LSR", "BBR5",
                            "RTS", "ADC", "STZ", "ADC", "ROR", "RMB6", "PLA", "ADC", "ROR", "JMP", "ADC", "ROR", "BBR6",
                            "BVS", "ADC", "ADC", "STZ", "ADC", "ROR", "RMB7", "SEI", "ADC", "PLY", "JMP", "ADC", "ROR", "BBR7",
                            "BRA", "STA", "STY", "STA", "STZ", "SMB0", "DEY", "BIT", "TXA", "STY", "STA", "STX", "BBS0",
                            "BCC", "STA", "STA", "STY", "STA", "STZ", "SMB1", "TYA", "STA", "TSX", "STZ", "STA", "STZ", "BBS1",
                            "LDY", "LDA", "LDX", "LDY", "LDA", "LDX", "SMB2", "TAY", "LDA", "TAX", "LDY", "LDA", "LDX", "BBS2",
                            "BCS", "LDA", "LDA", "LDY", "LDA", "LDX", "SMB3", "CLV", "LDA", "TSX", "LDY", "LDA", "LDX", "BBS3",
                            "CPY", "CMP", "CPY", "CMP", "DEC", "SMB4", "INY", "CMP", "DEX", "WAI", "CPY", "CMP", "DEC", "BBS4",
                            "BNE", "CMP", "CMP", "CMP", "DEC", "SMB5", "CLD", "CMP", "PHX", "STP", "CMP", "DEC", "BBS5",
                            "CPX", "SBC", "CPX", "SBC", "INC", "SMB6", "INX", "SBC", "NOP", "CPX", "SBC", "INC", "BBS6",
                            "BEQ", "SBC", "SBC", "SBC", "INC", "SMB7", "SED", "SBC", "PLX", "SBC", "INC", "BBS7"};  // Instruction names


// Clock interrupt
void onClock()
{

  // Monitor address pins
  int addr = 0;

  for (int i = 0; i < 16; i++) {
    int bit = (digitalRead(ADDR_PINS[i]) ? 1 : 0);   // Read the pin bit
    Serial.print(bit);
    addr = (addr << 1) + bit;    // Add the bit to the address
  }

  Serial.print("    ");


  // Monitor data pins
  int data = 0;

  for (int i = 0; i < 8; i++) {
    int bit = (digitalRead(DATA_PINS1[i]) ? 1 : 0);   // Read the pin bit
    Serial.print(bit);
    data = (data << 1) + bit;    // Add the bit to the data
  }

  Serial.print("    ");

  // Monitor read/write pin
  char rw = (digitalRead(RW_PIN) ? 'R' : 'W');

  // Print hexadecimal values
  char hex[20];
  sprintf(hex, "    %c    %04X    %02X", rw, addr, data);
  Serial.print(hex);

  // Print instruction
  int instr_len = sizeof(INSTR_OPC) / sizeof(int);
  int instr_flag = false;
  Serial.print("    ");

  for (int i = 0; i < instr_len; i++) {

    if (data == INSTR_OPC[i]) {
      instr_flag = true;
      Serial.print(INSTR_NAME[i]);
    }
  }

  if (not instr_flag) {
    Serial.print("---");
  }

  // Print bookmark
  if (data == 0xEA) {  // NOP OpCode
    Serial.print("   ---> BOOKMARK");
  }

  // Print reset
  if (addr == 0xFFFC) {  // Reset vector address
    Serial.print("   ---> RESET");
  }

  Serial.println("");

  // Halt clock
  if (HALT && (data == 0xEA)) {  // NOP OpCode
    digitalWrite(HALT_PIN, HIGH);   // Set high
  }
}


// Setup
void setup()
{
  // Serial port
  Serial.begin (9600);   // Set speed
  delay(2000);  // Wait for the serial port
  Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n==================================================\n");  // Clear screen

  // Clock pin
  pinMode(CLOCK_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), onClock, RISING);   // Attach an interrupt

  // Read/write pin
  pinMode(RW_PIN, INPUT_PULLUP);   // Set as input

  // Halt pin
  pinMode(HALT_PIN, OUTPUT);   // Set as output
  digitalWrite(HALT_PIN, LOW);   // Set low
  
  // Address pins
  for (int i = 0; i < 16; i++) {
    pinMode(ADDR_PINS[i], INPUT_PULLUP);   // Set as input
  }

  // Data pins
  for (int i = 0; i < 8; i++) {
    pinMode(DATA_PINS1[i], INPUT);   // Set as input
  }
}


// Loop
void loop()
{
}
