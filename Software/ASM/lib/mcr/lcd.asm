; LCD macros for the ITER-8.

; ==========================================================

; Stop an instruction.
;
; Registers modified:
; A
;
; Requirements:
; par:var_hw.asm

    .MACRO          LCD_INSTRUCT_STOP

; Stop instruction
    LDA             #%00000000
    STA             VIA_RA

    .ENDMACRO

; ==========================================================

; Send an instruction to the LCD.
;
; Arguments:
; INSTRUCT: instruction (1 B)
;
; Registers modified:
; A
;
; Requirements:
; par:var_hw.asm

    .MACRO          LCD_INSTRUCT INSTRUCT

; Send instruction
    JSR             LCD_WAIT                                ; Wait for previous instruction
    LDA             #INSTRUCT
    STA             VIA_RB                                  ; Send instruction
    LDA             #LCD_E                                  ; Enable LCD
    STA             VIA_RA
    LCD_INSTRUCT_STOP                                       ; Stop instruction

    .ENDMACRO

; ==========================================================

; Set VIA register B as input.
;
; Registers modified:
; A
;
; Requirements:
; par:var_hw.asm

    .MACRO          LCD_VIA_B_IN

; Set VIA register B as input
    LDA             #%00000000
    STA             VIA_DDRB

    .ENDMACRO

; ==========================================================

; Set VIA register B as outut.
;
; Registers modified:
; A
;
; Requirements:
; par:var_hw.asm

    .MACRO          LCD_VIA_B_OUT

; Set VIA register B as output
    LDA             #%11111111
    STA             VIA_DDRB

    .ENDMACRO

; ==========================================================

; Setup the LCD.
;
; Arguments:
; CURSOR: show cursor. '#0' for not blinking, '#1' for blinking (optional) (1 B)
;
; Registers modified:
; A
;
; Requirements:
; par:var_hw.asm

    .MACRO          LCD_SETUP CURSOR
    .SCOPE

; Set VIA register B as output
    LCD_VIA_B_OUT

; Set part of VIA register A as output
    LDA             #%11100000
    STA             VIA_DDRA

; Clear LCD
    INSTRUCT_1      = %00000001
    LCD_INSTRUCT    INSTRUCT_1

; Set LCD function set to 8-bit mode, 2 lines, 5x8 font
    INSTRUCT_2      = %00111000
    LCD_INSTRUCT    INSTRUCT_2

; Set LCD display control to on, and set cursor
    .IFBLANK        CURSOR
    INSTRUCT_3      = %00001100                             ; Cursor off
    .ELSE
    .IF             (.XMATCH (CURSOR, 0))
    INSTRUCT_3      = %00001110                             ; Cursor on, not blinking
    .ELSEIF         (.XMATCH (CURSOR, 1))
    INSTRUCT_3      = %00001111                             ; Cursor on, blinking
    .ENDIF
    .ENDIF
    LCD_INSTRUCT    INSTRUCT_3

; Set LCD entry mode set to increment, no display shift
    INSTRUCT_4      = %00000110
    LCD_INSTRUCT    INSTRUCT_4

; Set cursor home
    INSTRUCT_5      = %00000010
    LCD_INSTRUCT    INSTRUCT_5

    .ENDSCOPE
    .ENDMACRO

; ==========================================================

; Display a character.
; If the argument is not specified, the character is loaded from A.
;
; Arguments:
; CHAR: character (1 B) (optional)
;
; Registers modified:
; A
;
; Requirements:
; par:var_hw.asm

    .MACRO          LCD_CHAR CHAR

; Display character
    PHA
    JSR             LCD_WAIT                                ; Wait for previous instruction
    PLA                                                     ; If character is not loaded from memory
    .IFNBLANK       CHAR                                    ; If character is loaded from memory
    LDA             #CHAR
    .ENDIF
    STA             VIA_RB                                  ; Send character
    LDA             #(LCD_RS | LCD_E)                       ; Send instruction to LCD
    STA             VIA_RA
    LCD_INSTRUCT_STOP                                       ; Stop instruction

    .ENDMACRO

; ==========================================================

; Display a string.
;
; Arguments:
; STRING: initial address of the string, ending in NULL (I) (1- B)
;
; Registers modified:
; A, X
;
; Requirements:
; par:var_hw.asm

    .MACRO          LCD_PRINT STRING
    .SCOPE

; Loop characters
    LDX             #0                                      ; Initialize index
LOOP:
    LDA             STRING, X                               ; Load character
    BEQ             EXIT                                    ; If it's NULL, exit loop
    LCD_CHAR                                                ; Display character
    INX                                                     ; Increase index
    JMP             LOOP                                    ; Iterate

; Load registers
EXIT:

    .ENDSCOPE
    .ENDMACRO

; ==========================================================

; Convert a 8-bit binary number into a string in base 10.
;
; Arguments:
; B2:  address of the binary number (I) (1 B)
; B10:  address of the base-10 string ending in NULL (O) (2-4 B)
; QUOT: address of the temporary quotient (T) (1 B)
; REMAIN: address of the temporary remainder (T) (1 B)
;
; Requirements:
; par:char_map.asm
; mcr:math_arithm.asm
; mcr:utils.asm
;
; Registers modified:
; A, X

    .MACRO          BASE10_8 B2, B10, QUOT, REMAIN
    .SCOPE

; Prepare string
    LDA             #0
    STA             B10                                     ; Put NULL in string

; Save input
    LDA             B2
    PHA

; Loop digits
LOOP:
    DIVIDE_8        B2, #10, QUOT, REMAIN                   ; Divide by 10
    LDA             REMAIN
    ADC             #CHAR_0                                 ; Add remainder to the "0" ASCII value
    STA             REMAIN
    ADD_SEQ         B10, REMAIN                             ; Add digit to string
    LDA             QUOT                                    ; Save quotient
    STA             B2
    BNE             RANGE                                   ; If it's not the last digit, loop back
    JMP             EXIT
RANGE:                                                      ; Long range jump
    JMP             LOOP

; Restore input
EXIT:
    PLA
    STA             B2

    .ENDSCOPE
    .ENDMACRO

; ==========================================================

; Convert a 16-bit binary number into a string in base 10.
;
; Arguments:
; B2:  initial address of the binary number (I) (2 B)
; B10:  initial address of the base-10 string ending in NULL (O) (2-6 B)
; QUOT: initial address of the temporary quotient (T) (2 B)
; REMAIN: initial address of the temporary remainder (T) (2 B)
;
; Requirements:
; par:char_map.asm
; mcr:math_arithm.asm
; mcr:utils.asm
;
; Registers modified:
; A, X, Y

    .MACRO          BASE10_16 B2, B10, QUOT, REMAIN
    .SCOPE

; Prepare string
    LDA             #0
    STA             B10                                     ; Put NULL in string

; Save input
    LDA             B2
    PHA
    LDA             B2 + 1
    PHA

; Loop digits
LOOP:
    DIVIDE_16       B2, #10, QUOT, REMAIN                   ; Divide by 10
    LDA             REMAIN
    ADC             #CHAR_0                                 ; Add remainder to the "0" ASCII value
    STA             REMAIN
    ADD_SEQ         B10, REMAIN                             ; Add digit to string
    LDA             QUOT + 1                                ; Save quotient
    STA             B2 + 1
    LDA             QUOT
    STA             B2
    ORA             QUOT + 1                                ; Check if quotient is zero
    BNE             RANGE                                   ; If it's not the last digit, loop back
    JMP             EXIT
RANGE:                                                      ; Long range jump
    JMP             LOOP

; Restore input
EXIT:
    PLA
    STA             B2 + 1
    PLA
    STA             B2

    .ENDSCOPE
    .ENDMACRO
