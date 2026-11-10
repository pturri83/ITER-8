; CPU subroutines for the ITER-8.

; ======================================================================================================================

; Delay the CPU for a number of cycles.
; The number of cycles delayed, including the subroutine jump and return commands, is
; n = (b * ((256 * A) + X)) + c
; The parameter 'b' is the number of cycles in each loop; its value is b = 9. The parameter 'c' is the number of
; overhead cycles (when A = 0 and X = 0); its value is c = 20.
; The number of cycles that can be delayed is in the range 20-589835.
; An increment of X by 1 causes a delay of 9 us (excluding the overhead of 20 us).
;
; Input registers:
;   A               number of large loops (each equal to 256 small loops)
;   X               number of small loops
;
; Registers modified:
;   A, X


delay_cycles:
.SCOPE

    .ALIGN          $0100                                             ; Subroutine in the same page for consistent b = 9

; Loop
loop:
    CPX             #1                                                ; Determine the carry bit if X was decreased by 1
    DEX                                                               ; Decrease X
    SBC             #0                                                ; Decrease A if the carry bit was used
    BCS             loop                                              ; Loop if the carry bit was not used
    RTS

.ENDSCOPE

; ======================================================================================================================

; Delay the CPU for one millisecond.
; The number of loops used in 'delay_cycles', including the subroutine jump and return commands, is
; n = floor((floor(clk_hz / 1000) - c - d) / b) = 107.
; The parameter 'd' is the number of overhead cycles in this subroutine, not counting those in 'delay_cicles'; its value
; is d = 16. The parameters 'b' and'c' are explained in 'delay_cycles'.
; This subroutine is contained in a single page--like 'delay_cycles'--for consistent b = 9.
;
; Functions required:
;   delay_cycles
;
; Registers modified:
;   A, X


delay_ms:
.SCOPE

; Delay
    b               = 9                                               ; See docstring of 'delay_cycles'
    c               = 20                                              ; See docstring of 'delay_cycles'
    d               = 16                                              ; Overhead cycles
    n_x             = ((clk_hz / 1000) - c - d) / b                   ; Number of small loops in 1 ms
    LDA             #0                                                ; No large loops in 'delay_cycles'
    LDX             #n_x                                              ; Small loops in 'delay_cycles'
    JSR             delay_cycles
    RTS

.ENDSCOPE

; ======================================================================================================================

; Delay the CPU for a number of millisecond.
; The maximum number of milliseconds that can be delayed is 255.
;
; Functions required:
;   delay_ms
;   delay_cycles
;
; Input registers:
;   Y               number of milliseconds
;
; Registers modified:
;   A, X, Y


delay_mss:
.SCOPE

; Delay
loop:
    JSR             delay_ms                                          ; Delay 1 ms
    DEY                                                               ; Decrease Y
    BNE             loop                                              ; Loop if there are milliseconds left
    RTS

.ENDSCOPE
