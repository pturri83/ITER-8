; Generate a ROM image for the ITER-8 to tests the VIA chip and the LCD.
; Count the number of times a button is pressed and display it on the LCD.

; ======================================================================================================================

; Variables
    debug           = 0                                               ; Debug step-by-step (0 = False, 1 = True)

; ======================================================================================================================

; Libraries
.SEGMENT            "LIB"
    .INCLUDE        "test_via_lcd.inc"

; ======================================================================================================================

; Header
.SEGMENT            "HEAD"
    .ASCIIZ         "VIA-LCD test for the ITER-8 computer"            ; Title
    .ASCIIZ         "- Paolo Turri"                                   ; Author
    .ASCIIZ         "- 2025"                                          ; Year
    .ASCIIZ         "- MPL 2.0"                                       ; License
    .ASCIIZ         .SPRINTF("- UNIX time %d", .TIME)                 ; Time

; ======================================================================================================================

; Debug
.SEGMENT            "DEBUG"
.IF                 debug = 1                                         ; If debugging
    .ASCIIZ         "Debugged variables"
.ENDIF

; ======================================================================================================================

; Data
.RODATA
    flags_p:        .ASCIIZ "NV-BDIZC"                                ; Initials of flags in register P
    counter_str:    .ASCIIZ "Counter"                                 ; String for counter
    dividend: .BYTE 247
    divisor: .BYTE 35
    quotient: .BYTE 0
    remainder: .BYTE 0
    dividend21: .BYTE .LOBYTE(3107)
    dividend22: .BYTE .HIBYTE(3107)
    divisor21: .BYTE .LOBYTE(260)
    divisor22: .BYTE .HIBYTE(260)
    quotient21: .BYTE 0
    quotient22: .BYTE 0
    remainder21: .BYTE 0
    remainder22: .BYTE 0


.DATA

; ======================================================================================================================

; Adresses
.ZEROPAGE
    ptr_ram_1:      .RES 2                                            ; RAM pointer
    ptr_ram_2:      .RES 2                                            ; RAM counter pointer
    ptr_rom:        .RES 2                                            ; ROM pointer
    ptr_str:        .RES 2                                            ; String pointer
    ptr_seq:        .RES 2                                            ; Bytes sequnce pointer
    ptr_add:        .RES 2                                            ; Addition pointer
    ptr_add_2:        .RES 2
    ptr_sub:        .RES 2                                            ; Subtraction pointer
    ptr_sub_2:        .RES 2
    ptr_mult:        .RES 2                                            ; Multiplication pointer
    ptr_mult_2:        .RES 2
    ptr_div:        .RES 2                                            ; Division pointer
    ptr_div_2:        .RES 2


.BSS
    ; counter:        .RES 1                                            ; Button counter
    ; counter_str:    .RES 4                                            ; First term string
    division:  .RES 4
    division2:  .RES 8
    sequence:  .RES 4
    sequence2:  .RES 6
    string:  .RES 4

; ======================================================================================================================

; Vectors
.SEGMENT            "VECTORS"
    .ADDR           nmib                                              ; Non-maskable interrupt request
    .ADDR           reset                                             ; Reset
    .ADDR           irqb                                              ; Interrupt request

; ======================================================================================================================

; Main code
.CODE

; Initialize hardware
reset:
    cpu_init                                                          ; Reset CPU
    ram_init                                                          ; Reset RAM
    via_init                                                          ; Reset VIA
    lcd_init                                                          ; Reset LCD


    LDA             #char_x             ; remove this block, it's only for testing the LCD, and with the original clock
    JSR             lcd_char


    ; Initialize text
    ; debug_nop
    ; LDA             #.LOBYTE(counter_str)                             ; Low byte of initial adress of string
    ; STA             ptr_str
    ; LDA             #.HIBYTE(counter_str)                             ; High byte of initial adress of string
    ; STA             ptr_str + 1
    ; JSR             lcd_print                                         ; Display string
    ; LDA             char_colon                                        ; Display colon character
    ; JSR             lcd_char
    ; JSR             lcd_home_2                                         ; Second line
    ; debug_nop

; Initialize counter           ; can i enable interrupts after this part of the code
    ; LDA             #0                                                ; Initialize button counter
    ; STA             counter


; division test
    LDA dividend
    STA division
    LDA divisor
    STA division + 1
    LDA quotient
    STA division + 2
    LDA remainder
    STA division + 3
    LDA             #.LOBYTE(division)
    STA ptr_div
    LDA             #.HIBYTE(division)
    STA ptr_div + 1
    JSR divide_8
    LDA division
    LDA division + 1
    LDA division + 2
    LDA division + 3
    debug_nop
    LDA             #.LOBYTE(sequence)
    STA ptr_seq
    LDA             #.HIBYTE(sequence)
    STA ptr_seq + 1
    LDA division
    JSR convert_8_10
    debug_nop
    LDA (ptr_seq)
    LDY 1
    LDA (ptr_seq), Y
    LDY 2
    LDA (ptr_seq), Y
    LDY 3
    LDA (ptr_seq), Y
    debug_nop
    LDA             #.LOBYTE(string)
    STA ptr_str
    LDA             #.HIBYTE(string)
    STA ptr_str + 1
    JSR convert_10_ascii
    LDA (ptr_str)
    LDY 1
    LDA (ptr_str), Y
    LDY 2
    LDA (ptr_str), Y
    LDY 3
    LDA (ptr_str), Y
    debug_nop
    JSR lcd_print
    debug_nop
    JSR     debug_lcd
    debug_nop
    TXA
    TYA
    debug_nop
    LDA dividend21
    STA division2
    LDA dividend22
    STA division2 + 1
    LDA divisor21
    STA division + 2
    LDA divisor22
    STA division + 3
    LDA quotient21
    STA division + 4
    LDA quotient22
    STA division + 5
    LDA remainder21
    STA division + 6
    LDA remainder22
    STA division + 7
    LDA             #.LOBYTE(division2)
    STA ptr_div_2
    LDA             #.HIBYTE(division2)
    STA ptr_div_2 + 1
    JSR divide_16
    debug_nop
    LDA division
    LDA division + 1
    LDA division + 2
    LDA division + 3
    LDA division + 4
    LDA division + 5
    LDA division + 6
    LDA division + 7
    debug_nop



    ; test all math operations


; ; Display counter
; display:

; ; Convert counter to string
;     SEI                                                               ; Disable interrupts
; ;     base10_16       divid, divid_str, quot_tmp, remain_tmp            ; Convert dividend        clean and make it a subroutine
;     CLI                                                               ; Enable interrupts

; ; Display counter
;     JSR             lcd_home                                          ; Return cursor    ;maybe i have also to clear the screen after 255, otherwise the screen might show "055"
;     LDA             char_space                                        ; Display space character
;     JSR             lcd_char
;     LDA             char_hash                                         ; Display hash character
;     JSR             lcd_char
;     LDA             char_space                                        ; Display space character
;     JSR             lcd_char
;     ; lcd_print       counter_str                                         ; Display dividend         clean and make it a subroutine

; Idle
    idle_loop                                                         ; Idle loop

; Non-maskable interrupt code
    .ALIGN          $0100
nmib:
    RTI                                                               ; Exit interrupt

; Interrupt code
    .ALIGN          $0100
irqb:
    ; INC             counter                                           ; Increment button counter
    JSR             delay_ms                                          ; Debounce button       ; check if 1 ms is enough
    via_clear                                                         ; Clear interrupt
    RTI                                                               ; Exit interrupt

; ======================================================================================================================

; Messages
    .OUT            "\nCode compilation terminated\n"                 ; Termination message
    .ASSERT         debug = 0, warning, \
                    "The code was compiled in DEBUG mode\n"           ; Debug message
