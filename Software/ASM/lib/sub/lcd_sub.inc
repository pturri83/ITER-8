; LCD subroutines for the ITER-8.

; ======================================================================================================================

; Wait for the LCD to finish an instruction by checking repeatedly the busy flag.
;
; Registers modified:
;   A


lcd_wait:
.SCOPE

; Change VIA register B direction
    dir_other       = (.BITNOT via_lcd) .BITAND via_outb              ; Direction of non-LCD pins
    dir_all         = lcd_rs .BITOR lcd_rwb .BITOR lcd_e .BITOR \
                    dir_other                                         ; Direction for reading LCD nibble
    LDA             #dir_all                                          ; Enable reading LCD nibble
    STA             via_ddrb_a

; Wait for busy flag
loop:
    LDA             #lcd_rwb                                          ; Read LCD
    STA             via_rb_a
    ORA             #lcd_e                                            ; Enable LCD
    STA             via_rb_a
    LDA             via_rb_a                                          ; Read high nibble
    PHA                                                               ; Store high nibble
    LDA             #lcd_rwb                                          ; Disenable LCD
    STA             via_rb_a
    ORA             #lcd_e                                            ; Enable LCD
    STA             via_rb_a
    LDA             via_rb_a                                          ; Read low nibble
    LDA             #lcd_rwb                                          ; Disenable LCD
    STA             via_rb_a
    PLA                                                               ; Restore high nibble
    AND             #%10000000                                        ; Isolate busy flag
    BNE             loop                                              ; If busy flag is on, loop back

; Restore VIA register B direction
    LDA             #via_outb                                         ; Default data direction of port B
    STA             via_ddrb_a
    RTS

.ENDSCOPE

; ======================================================================================================================

; Send an instruction to the LCD using 8 bit mode and without checking the busy flag.
; This command is useful to communicate to the LCD during initialization, before it is set to 4-bit mode. It requires
; to manually set some time delay after the previous instruction, since 'lcd_wait' is not yet available.
;
; Input registers:
;   A               LCD instruction
;
; Registers modified:
;   A


lcd_instruct_nowait:
.SCOPE

; Send instruction
    STA             via_rb_a                                          ; Send instruction
    ORA             #lcd_e                                            ; Enable LCD
    STA             via_rb_a
    EOR             #lcd_e                                            ; Disenable LCD
    STA             via_rb_a
    RTS

.ENDSCOPE

; ======================================================================================================================

; Send an instruction to the LCD, first waiting for the busy flag to clear.
;
; Functions required:
;   lcd_wait
;
; Symbols required:
;   debug           debug step-by-step (0 = False, 1 = True)
;
; Input registers:
;   A               LCD instruction
;
; Registers modified:
;   A


lcd_instruct:
.SCOPE

; Send high nibble
    PHA                                                               ; Store instruction

.IF                 ::debug = 0                                       ; If not debugging
    JSR             lcd_wait                                          ; Wait for the busy flag to clear
.ENDIF

    PLA                                                               ; Restore instruction
    PHA                                                               ; Store instruction
    AND             #%11110000                                        ; Select high nibble
    STA             via_rb_a
    ORA             #lcd_e                                            ; Enable LCD
    STA             via_rb_a
    EOR             #lcd_e                                            ; Disenable LCD
    STA             via_rb_a

; Send low nibble
.IF                 ::debug = 0                                       ; If not debugging
    JSR             lcd_wait                                          ; Wait for the busy flag to clear
.ENDIF

    PLA                                                               ; Restore instruction
    AND             #%00001111                                        ; Select low nibble
    ASL             A                                                 ; Shift nibble
    ASL             A                                                 ; Shift nibble
    ASL             A                                                 ; Shift nibble
    ASL             A                                                 ; Shift nibble
    STA             via_rb_a
    ORA             #lcd_e                                            ; Enable LCD
    STA             via_rb_a
    EOR             #lcd_e                                            ; Disenable LCD
    STA             via_rb_a
    RTS

.ENDSCOPE

; ======================================================================================================================

; Return the cursor at the beginning of the first line.
;
; Functions required:
;   lcd_instruct
;   lcd_wait
;
; Symbols required:
;   debug           debug step-by-step (0 = False, 1 = True)
;
; Registers modified:
;   A


lcd_home:
.SCOPE

; Return cursor home
    LDA             #%00000010                                        ; Return cursor home
    JSR             lcd_instruct
    RTS

.ENDSCOPE

; ======================================================================================================================

; Return the cursor at the beginning of the second line.
;
; Functions required:
;   lcd_instruct
;   lcd_wait
;
; Symbols required:
;   debug           debug step-by-step (0 = False, 1 = True)
;
; Registers modified:
;   A


lcd_home_2:
.SCOPE

; Return cursor at the beginning of second line
    LDA             #%11000000                                        ; Return cursor at the beginning of second line
    JSR             lcd_instruct
    RTS

.ENDSCOPE

; ======================================================================================================================

; Clear the display.
;
; Functions required:
;   lcd_instruct
;   lcd_wait
;
; Symbols required:
;   debug           debug step-by-step (0 = False, 1 = True)
;
; Registers modified:
;   A


lcd_clear:
.SCOPE

; Clear display
    LDA             #%00000001                                        ; Clear display
    JSR             lcd_instruct
    RTS

.ENDSCOPE

; ======================================================================================================================

; Display a character on the LCD data register in 4-bit mode.
;
; Functions required:
;   lcd_wait
;
; Symbols required:
;   debug           debug step-by-step (0 = False, 1 = True)
;
; Input registers:
;   A               Character code
;
; Registers modified:
;   A


lcd_char:
.SCOPE

; Send high nibble
    PHA                                                               ; Store data

.IF                 ::debug = 0                                       ; If not debugging
    JSR             lcd_wait                                          ; Wait for the busy flag to clear
.ENDIF

    PLA                                                               ; Restore data
    PHA                                                               ; Store data
    AND             #%11110000                                        ; Select high nibble
    ORA             #lcd_rs                                           ; Select data register
    STA             via_rb_a
    ORA             #lcd_e                                            ; Enable LCD
    STA             via_rb_a
    EOR             #lcd_e                                            ; Disenable LCD
    STA             via_rb_a

; Send low nibble
    PLA                                                               ; Restore data
    ASL             A                                                 ; Shift nibble
    ASL             A                                                 ; Shift nibble
    ASL             A                                                 ; Shift nibble
    ASL             A                                                 ; Shift nibble
    ORA             #lcd_rs                                           ; Select data register
    STA             via_rb_a
    ORA             #lcd_e                                            ; Enable LCD
    STA             via_rb_a
    EOR             #lcd_e                                            ; Disenable
    STA             via_rb_a
    RTS

.ENDSCOPE

; ======================================================================================================================

; Display a string on the LCD.
;
; Instructions:
;   - The input string has to be stored in '*' and it has to end in 'NULL'.
;   - The starting adress of '*' has to be loaded in 'ptr_str'.
;
; Functions required:
;   lcd_char
;   lcd_wait
;
; Symbols required:
;   debug           debug step-by-step (0 = False, 1 = True)
;
; Zero page pointers:
;   ptr_str
;
; Input memory:
;   * (n B)         string
;
; Registers modified:
;   A, Y


lcd_print:
.SCOPE

; Loop characters
    LDY             #0                                                ; Initialize character index
loop:
    LDA             (ptr_str), Y                                      ; Load character
    BEQ             exit                                              ; If it is 'NULL', exit loop
    JSR             lcd_char                                          ; Display character
    INY                                                               ; Increase character index
    JMP             loop                                              ; Iterate
exit:
    RTS

.ENDSCOPE

; ======================================================================================================================

; Display a nibble on the LCD as a hexadecimal digit. The nibble is the 4 least significant bits of the byte
;
; Functions required:
;   lcd_char
;   lcd_wait
;
; Symbols required:
;   debug           debug step-by-step (0 = False, 1 = True)
;
; Input registers:
;   A               Byte with the nibble to display in the LSBs
;
; Registers modified:
;   A


lcd_nibble:
.SCOPE

; Display low nibble
    AND             #%00001111                                        ; Select low nibble
    ORA             #char_0                                           ; Add nibble to character "0"
    CMP             #char_9 + 1                                       ; Check if it is past character "9"
    BCC             print                                             ; If it's a numerical character, skip to printing
    ADC             #6                                                ; Select an alphabetic character
print:
    JSR             lcd_char                                          ; Print the character
    RTS

.ENDSCOPE

; ======================================================================================================================

; Display a byte on the LCD as a couple of hexadecimal digit.
;
; Functions required:
;   lcd_nibble
;   lcd_char
;   lcd_wait
;
; Symbols required:
;   debug           debug step-by-step (0 = False, 1 = True)
;
; Input registers:
;   A               Byte to display
;
; Registers modified:
;   A


lcd_byte:
.SCOPE

; Display high nibble
    PHA                                                               ; Store byte
    LSR             A                                                 ; Shift nibble
    LSR             A                                                 ; Shift nibble
    LSR             A                                                 ; Shift nibble
    LSR             A                                                 ; Shift nibble
    JSR             lcd_nibble                                        ; Display nibble

; Display low nibble
    PLA                                                               ; Restore byte
    JSR             lcd_nibble                                        ; Display nibble
    RTS

.ENDSCOPE
