; LCD subroutines for the ITER-8.

; ==========================================================

; Wait for LCD to finish an instruction by checking its busy flag.
;
; Registers modified:
; A
;
; Requirements:
; par:var_hw.asm
; mcr:lcd.asm

LCD_WAIT:
    .SCOPE

; Set VIA register B as input
    LCD_VIA_B_IN

; Wait loop
LOOP:
    LDA             #LCD_RWB                                ; Read busy flag
    STA             VIA_RA
    LDA             #(LCD_RWB | LCD_E)                      ; Send instruction to LCD
    STA             VIA_RA
    LDA             VIA_RB                                  ; Load LCD data
    AND             #%10000000                              ; Isolate first bit (busy flag)
    BNE             LOOP                                    ; If busy flag is on, read it gain again
    LCD_INSTRUCT_STOP                                       ; Stop instruction

; Set VIA register B as output
    LCD_VIA_B_OUT
    RTS

    .ENDSCOPE

; ==========================================================

; Move cursor to the beginning of the second line.
;
; Registers modified:
; A
;
; Requirements:
; par:var_hw.asm
; mcr:lcd.asm

LCD_LINE2:
    .SCOPE

; Move to second line
    INSTRUCT        = %11000000
    LCD_INSTRUCT    INSTRUCT
    RTS

    .ENDSCOPE
