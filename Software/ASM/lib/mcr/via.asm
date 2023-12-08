; VIA chip macros for the ITER-8.

; ==========================================================

; Initialize the VIA chip.
;
; Registers modified:
; A
;
; Requirements:
; par:var_hw.asm

    .MACRO          VIA_INIT

; Set interrupts
    LDA             #%10000010                              ; Enable interrupts on CA1
    STA             VIA_IER
    LDA             #%00000000                              ; Set CA1 interrupts on negative active edge
    STA              VIA_PCR

    .ENDMACRO
