; LCD subroutines for the ITER-8.

; ==========================================================

; Wait for LCD to finish an instruction by checking its busy flag.
;
; Requirements:
; par:var_hw.asm

LCD_WAIT:
    .SCOPE

; Save registers
    PHA

; Set VIA register B direction
    LDA             #%00001110                              ; Data bits as input
    STA             VIA_DDRB

; Wait for busy flag
LOOP:
    LDA             #LCD_RWB                                ; Read
    STA             VIA_RB                                  ; Send instruction
    ORA             #LCD_E                                  ; Enable
    STA             VIA_RB
    LDA             VIA_RB                                  ; Read high nibble
    PHA                                                     ; Store high nibble
    LDA             #LCD_RWB                                ; Read
    EOR             #LCD_E                                  ; Stop enable
    STA             VIA_RB
    ORA             #LCD_E                                  ; Enable
    STA             VIA_RB
    EOR             #LCD_E                                  ; Stop enable
    STA             VIA_RB
    PLA                                                     ; Restore high nibble
    AND             #%10000000                              ; Isolate busy flag
    BNE             LOOP                                    ; If busy flag is on, loop back

; Set VIA register B direction
    LDA             #%11111110                              ; Data bits as output
    STA             VIA_DDRB

; Load registers
    PLA
    RTS

    .ENDSCOPE

; ==========================================================

; Send an instruction to the LCD data register while in 4-bit mode, checking first the busy flag.
; This function can be used to display a character.
;
; Registers:
; A: Instruction
;
; Registers modified:
; A
;
; Requirements:
; par:var_hw.asm

LCD_DATA:
    .SCOPE

; Send high nibble
    JSR             LCD_WAIT                                ; Wait for previous instruction to complete
    PHA                                                     ; Store instruction
    AND             #%11110000                              ; Isolate high nibble
    ORA             #LCD_RS                                 ; Select data register
    STA             VIA_RB
    ORA             #LCD_E                                  ; Enable
    STA             VIA_RB
    EOR             #LCD_E                                  ; Stop enable
    STA             VIA_RB

; Send low nibble
    PLA                                                     ; Restore instruction
    ASL             A                                       ; Shift instruction
    ASL             A                                       ; Shift instruction
    ASL             A                                       ; Shift instruction
    ASL             A                                       ; Shift instruction
    ORA             #LCD_RS                                 ; Select data register
    STA             VIA_RB                                  ; Send instruction
    ORA             #LCD_E                                  ; Enable
    STA             VIA_RB
    EOR             #LCD_E                                  ; Stop enable
    STA             VIA_RB
    RTS

    .ENDSCOPE

; ==========================================================

; Send an instruction to the LCD instruction register while in 4-bit mode, checking first the busy flag.
;
; Registers:
; A: Instruction
;
; Registers modified:
; A
;
; Requirements:
; par:var_hw.asm

LCD_INSTRUCT:
    .SCOPE

; Send high nibble
    JSR             LCD_WAIT                                ; Wait for previous instruction to complete
    PHA                                                     ; Store instruction
    AND             #%11110000                              ; Isolate high nibble
    STA             VIA_RB                                  ; Send instruction
    ORA             #LCD_E                                  ; Enable
    STA             VIA_RB
    EOR             #LCD_E                                  ; Stop enable
    STA             VIA_RB

; Send low nibble
    JSR             LCD_WAIT                                ; Wait for previous instruction to complete
    PLA                                                     ; Restore instruction
    ASL             A                                       ; Shift instruction
    ASL             A                                       ; Shift instruction
    ASL             A                                       ; Shift instruction
    ASL             A                                       ; Shift instruction
    STA             VIA_RB                                  ; Send instruction
    ORA             #LCD_E                                  ; Enable
    STA             VIA_RB
    EOR             #LCD_E                                  ; Stop enable
    STA             VIA_RB
    RTS

    .ENDSCOPE

; ==========================================================

; Send an instruction to the LCD while in 8-bit mode, without checking the busy flag.
;
; Registers:
; A: Instruction
;
; Registers modified:
; A
;
; Requirements:
; par:var_hw.asm

LCD_INSTRUCT_NOWAIT:
    .SCOPE

; Send instruction
    STA             VIA_RB                                  ; Send instruction
    ORA             #LCD_E                                  ; Enable
    STA             VIA_RB
    EOR             #LCD_E                                  ; Stop enable
    STA             VIA_RB
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

LCD_LINE_2:
    .SCOPE

; Move to second line
    LDA             #%11000000
    JSR             LCD_INSTRUCT
    RTS

    .ENDSCOPE
