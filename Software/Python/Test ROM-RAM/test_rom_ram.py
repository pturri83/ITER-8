"""Generate a ROM image for the ITER-8 to test the ROM and RAM. The test is run
step-by-step, with a bus analyzer.

Store to and load from RAM the A and X registers.
"""


from os import path


# Assembly code
code = bytearray(
    [0xA9, 0b01010101,  # LDA #$55
     # Load alternating bits (starting with 0) in the CPU A register
     0xA2, 0b10101010,  # LDX #$AA
     # Load alternating bits (starting with 1) in the CPU X register
     0x8D, 0x00, 0x02,  # STA $0200
     # Store the CPU A register in RAM
     0x8E, 0x01, 0x02,  # STX $0201
     # Store the CPU X register in RAM
     0xAD, 0x00, 0x02,  # LDA $0200
     # Load from RAM to the CPU A register
     0xAE, 0x01, 0x02,  # LDX $0201
     # Load from RAM to the CPU X register
     ])

# Generate binary
rom = code + bytearray([0xEA] * (32768 - len(code)))  # Fill with NOP
rom[0x7FFC] = 0x00; rom[0x7FFD] = 0x80  # Reset vector

# Save binary
path = \
    path.join(path.dirname(path.dirname(path.dirname(path.abspath(__file__)))),
              'Binary', 'test_rom_ram.bin')

with open(path, 'wb') as rom_file:
    rom_file.write(rom)
