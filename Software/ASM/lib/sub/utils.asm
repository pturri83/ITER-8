; Utility subroutines for the ITER-8.

; ==========================================================

; The CPU idles in an endless empty loop.

IDLE_LOOP:

    JMP             IDLE_LOOP

; ==========================================================

; Delay for a number of cycles.
; The number of cycles delayed, including the subroutine jump and return commands is
; n = c * ((256 * A) + X) + 20
; with c = 9 if the LOOP jump is in the same page (most likely), otherwise c = 10.
; The available range of cycles is 20-592148 (20-657940 if in different pages).
;
; Registers:
; A: large loop counter (x256)
; X: small loop counter (x1)
;
; Registers modified:
; A, X

DELAY_CYCLES:
    .SCOPE

; Loop
LOOP:
    CPX             #1                                      ; Calculate carry bit when decreasing X
    DEX                                                     ; Decrease X
    SBC             #0                                      ; Decrease A, if the carry bit was used
    BCS             LOOP                                    ; Loop back if the carry bit was not used in decreasing A
    RTS

    .ENDSCOPE

; ==========================================================

; Delay for one millisecond, including the subroutine jump and return commands.
;
; Requirements:
; par:var_hw.asm
;
; Registers modified:
; A, X

DELAY_MS:
    .SCOPE

; Delay
    CYCLES          = ((CLK_HZ / 1000) - (20 + 16)) / 9     ; Number of cycles in one millisecond
    LDA             #0
    LDX             #CYCLES
    JSR             DELAY_CYCLES
    RTS

    .ENDSCOPE
