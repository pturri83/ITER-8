; Debug macros for the ITER-8.

; ======================================================================================================================

; Add three bytes ($12, $34, $56) to serve as a bookmark in the assembled bynary.
; The macro is assembled only in debug mode.
;
; Symbols required:
;   debug           debug step-by-step (0 = False, 1 = True)


.MACRO debug_book

; Prepare data
.IF                 ::debug = 1                                      ; If debugging
    .BYTE           $12
    .BYTE           $34
    .BYTE           $56
.ENDIF

.ENDMACRO

; ======================================================================================================================

; Add two consecutive NOP instruction to halt the clock through the Arduino.
; The macro is assembled only in debug mode.
;
; Symbols required:
;   debug           debug step-by-step (0 = False, 1 = True)


.MACRO debug_nop

; Prepare data
.IF                 debug = 1                                         ; If debugging
    NOP
    NOP
.ENDIF

.ENDMACRO

; ======================================================================================================================

; Store the value of a numerical variable as an ASCII string in the DEBUG memory segment.
; The number is automatically converted to the chosen base (10 or 16). The number of digits shown is always four.
; The macro is assembled only in debug mode.
;
; Instructions:
;   - Do not specify the arguments 'places' and 'divisor' when calling the macro. They are used for the recursion.
;
; Symbols required:
;   debug           debug step-by-step (0 = False, 1 = True)
;
; Input arguments:
;   var             variable
;   base            base of the variable (10 or 16)


.MACRO debug_var    var, base, places, divisor
.SCOPE

; Change segment
.PUSHSEG                                                              ; Store the current segment
.SEGMENT            "DEBUG"                                           ; Move to "DEBUG" segment

; Store the variable
.IF                 debug = 1                                         ; If debugging
.IFBLANK            places                                            ; First iteration
    .ASCIIZ         "-"
.IF                 base = 10
    .ASCIIZ         "0d"                                              ; Insert decimal prefix
.ELSEIF             base = 16
    .ASCIIZ         "0x"                                              ; Insert hexadecimal prefix
.ENDIF
    debug_var       var, base, 4, 1                                   ; Initialize 'places' and 'divisor'
.ELSE
.IF                 places > 0                                        ; Continue until last digit
    debug_var       var, base, (places - 1), (divisor * base)         ; Recursion
    digit           = (var / divisor) .MOD base                       ; Calculate digit
    .LOBYTES(digit + ((digit > 9) * $07) + $30)                       ; Convert digit to ASCII and store it   ; maybe i can use the subroutines to convert to decimal and then to ASCII
.ENDIF
.ENDIF
.ENDIF

; Change segment
.POPSEG                                                               ; Return to the previous segment

.ENDSCOPE
.ENDMACRO

; ======================================================================================================================

; Display debugging information on the LCD.     ;;;test
; The display shows the value of registers PC, P, A, X, Y, and S, as they were at the time of calling this macro.
;
; Functions required:
;   lcd_char
;   lcd_wait
;   lcd_home
;   lcd_instruct
;   lcd_home_2
;
; Symbols required:
;   debug           debug step-by-step (0 = False, 1 = True)
;
; Input memory:
;   flags_p (9 B)   string of initials of flags in register P


debug_lcd:
.SCOPE
; Calculate base stack adresses
    ; pc_base         = stack_s + 6                                     ; PC adress    program counter
    ; p_base          = stack_s + 5                                     ; P adress     flags
    ; a_base          = stack_s + 4                                     ; A adress
    ; x_base          = stack_s + 3                                     ; X adress
    ; y_base          = stack_s + 2                                     ; Y adress
    ; s_base          = 6                                               ; S adress     stack pointer

; Dump registers to stack
    PHP		                                                          ; Store P
    JSR             debug_pc
    ;subtract (add?) 1 from x, y  to account for the PHP instruction
	PHA                                                               ; Store A
	TXA
	PHA                                                               ; Store X
	TYA
	PHA                                                               ; Store Y
    TSX		                                                          ; Store S

; ; Display PC
;     JSR             lcd_clear                                         ; Clear display
;     JSR             lcd_home                                          ; Return cursor to the first line
; 	LDA             char_p                                            ; Display "P" character
;     JSR             lcd_char
;     LDA             char_c                                            ; Display "C" character
;     JSR             lcd_char
;     LDA             char_dollar                                       ; Display "$" character
;     JSR             lcd_char
;     LDA             pc_base, X                                        ; Load PC low byte
; 	LDY             pc_base+1, X	                                  ; Load PC high byte
; 	CLC		                                                          ; Clear carry bit
; 	ADC             #1		                                          ; Increase the PC low byte
; 	BCC             skip1                                             ; Skip if the PC low byte does not set the carry
;                                                                       ; flag
; 	INY                                                               ; Increase the PC high byte
; skip1:
;     PHA                                                               ; Store the PC low byte
;     TYA                                                               ; Load the PC high byte
; 	JSR             lcd_byte                                          ; Display hexadecimal high byte
;     PLA                                                               ; Restore the PC low byte
;     JSR             lcd_byte                                          ; Display hexadecimal low byte

; ; Display P
;     LDA             char_p
;     JSR             lcd_char                                          ; Display "P" character
;     LDY             #0                                                ; Flag counter
; 	LDA             p_base, X                                         ; Load P
; flag:
;     ROL             A                                                 ; Move the flag to C
; 	PHA                                                               ; Store remaining flags
; 	LDA             flags_p, Y                                        ; Load the character of the flag
; 	BCS             skip2                                             ; Skip if the flag is not set
;     LDA             char_dash                                         ; Load the "-" character
; skip2:
; 	JSR             lcd_char                                          ; Display character
; 	PLA                                                               ; Restore remaining flags
; 	INY                                                               ; Increase flag counter
; 	CPY             #8                                                ; Check if the flag is the last
; 	BNE             flag                                              ; Loop back if there are remaining flags

; ; Display A
;     JSR             lcd_home_2                                        ; Return cursor to the second line
; 	LDA             char_a
;     JSR             lcd_char                                          ; Display "A" character
;     LDA             char_dollar
;     JSR             lcd_char                                          ; Display "$" character
; 	LDA             a_base, X                                         ; Load A
; 	JSR             lcd_byte                                          ; Display hexadecimal byte

; ; Display X
; 	LDA             char_x
;     JSR             lcd_char                                          ; Display "X" character
;     LDA             char_dollar
;     JSR             lcd_char                                          ; Display "$" character
; 	LDA             x_base, X                                         ; Load X
; 	JSR             lcd_byte                                          ; Display hexadecimal byte

; ; Display Y
; 	LDA             char_y
;     JSR             lcd_char                                          ; Display "Y" character
;     LDA             char_dollar
;     JSR             lcd_char                                          ; Display "$" character
; 	LDA             y_base, X                                         ; Load X
; 	JSR             lcd_byte                                          ; Display hexadecimal byte

; ; Display S
; 	LDA             char_s
;     JSR             lcd_char                                          ; Display "S" character
;     LDA             char_dollar
;     JSR             lcd_char                                          ; Display "$" character
;     CLC                                                               ; Clear carry bit
; 	TXA                                                               ; Load S
; 	ADC             #s_base                                           ; Remove the other saved registers from S
; 	JSR             lcd_byte                                          ; Display hexadecimal byte

; Restore registers
    PLA
    TAY                                                               ; Restore Y
	PLA
	TAX                                                               ; Restore X
	PLA                                                               ; Restore A
	PLP                                                               ; Restore P
	RTS

.ENDSCOPE
