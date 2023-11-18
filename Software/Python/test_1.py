"""Script to create the first test ROM image for the ITER-8 using machine code.
The output file in the ROMs directory is called 'test_1.bin'.
"""


from os import path


# Code
code = bytearray(
    [0xA9, 0xFF,  # LDA #$FF; Prepare in Register A of CPU the direction of Register B of I/O as output
     0x8D, 0x02, 0x60,  # STA $6002; Store Register A of CPU in Register DDRB of I/O
     0xA9, 0x55,  # LDA #$55; Load #%01010101 in Register A of CPU
     0x8D, 0x00, 0x60,  # STA $6000; Store Register A of CPU in Register ORB of I/O
     0xA9, 0xaa,  # LDA #$AA; Load #%10101010 in Register A of CPU
     0x8D, 0x00, 0x60,  # STA $6000; Store Register A of CPU in Register ORB of I/O
     0x4C, 0x05, 0x80  # JMP $8005; Loop back
     ])

# Create the ROM
rom = code + bytearray([0xEA] * (32768 - len(code)))  # Fill with NOP
rom[0x7FFC] = 0x00; rom[0x7FFD] = 0x80  # Starting address

# Save the ROM
path = \
    path.join(path.dirname(path.dirname(path.dirname(path.abspath(__file__)))),
              'ROMs', 'test_1.bin')

with open(path, 'wb') as rom_file:
    rom_file.write(rom)
