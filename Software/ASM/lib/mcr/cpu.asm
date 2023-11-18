; CPU macros for the ITER-8.

; ==========================================================

; Setup the CPU.
; This has to be executed outside of subroutines, since it modifies the stack pointer.
;
; Requirements:
; par:var_hw.asm

    .MACRO          CPU_SETUP

; Initialize SP
    LDX             #CPU_SP
    TXS

    .ENDMACRO
