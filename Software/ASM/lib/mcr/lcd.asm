; LCD macros for the ITER-8.

; ==========================================================

; Initialize the LCD.
;
; Arguments:
; CURSOR: show cursor. '#0' for not blinking, '#1' for blinking (optional) (1 B)
;
; Registers modified:
; A, X, Y
;
; Requirements:
; par:var_hw.asm
; sub:lcd.asm

    .MACRO          LCD_INIT CURSOR
    .SCOPE

; Set VIA register B direction
    LDA             #%11111110                              ; Data bits as output
    STA             VIA_DDRB

; Set LCD
    LDY             #15                                     ; Wait 15 ms
    JSR             DELAY_MSS
    LDA             #%00110000                              ; Set LCD special function set
    JSR             LCD_INSTRUCT_NOWAIT
    LDY             #5                                      ; Wait 5 ms
    JSR             DELAY_MSS
    LDA             #%00110000                              ; Set LCD special function set
    JSR             LCD_INSTRUCT_NOWAIT
    JSR             DELAY_MS                                ; Wait 1 ms
    LDA             #%00110000                              ; Set LCD special function set
    JSR             LCD_INSTRUCT_NOWAIT
    JSR             DELAY_MS                                ; Wait 1 ms

; Set LCD to 4-bit mode, 2 lines, 5x8 font
    LDA             #%00100000
    JSR             LCD_INSTRUCT_NOWAIT                     ;try to remove nowait
    LDA             #%00101000
    JSR             LCD_INSTRUCT

; Set LCD on, cursor
    .IFBLANK        CURSOR
    LDA             #%00001100                              ; Cursor off
    .ELSE
    .IF             (.XMATCH (CURSOR, 0))
    LDA             #%00001110                              ; Cursor on, not blinking
    .ELSEIF         (.XMATCH (CURSOR, 1))
    LDA             #%00001111                              ; Cursor on, blinking
    .ENDIF
    .ENDIF
    JSR             LCD_INSTRUCT

; Set LCD entry mode set to increment, no display shift
    LDA             #%00000110
    JSR             LCD_INSTRUCT

; Set cursor home
    LDA             #%00000010
    JSR             LCD_INSTRUCT
    LDY             #2
    JSR             DELAY_MSS

; Clear LCD
    LDA             #%00000001
    JSR             LCD_INSTRUCT
    LDY             #2
    JSR             DELAY_MSS

    .ENDSCOPE
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
; sub:lcd.asm

    .MACRO          LCD_PRINT STRING
    .SCOPE

; Loop characters
    LDX             #0                                      ; Initialize index
LOOP:
    LDA             STRING, X                               ; Load character
    BEQ             EXIT                                    ; If it's NULL, exit loop
    JSR             LCD_DATA                                ; Display character
    INX                                                     ; Increase index
    JMP             LOOP                                    ; Iterate
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
; Registers modified:
; A, X
;
; Requirements:
; par:char_map.asm
; mcr:math_arithm.asm
; mcr:utils.asm

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
; Registers modified:
; A, X, Y
;
; Requirements:
; par:char_map.asm
; mcr:math_arithm.asm
; mcr:utils.asm

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
