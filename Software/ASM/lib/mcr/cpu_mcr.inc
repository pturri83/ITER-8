; CPU macros for the ITER-8.

; ======================================================================================================================

; Initialize the CPU, with interrupts disabled.
;
; Instructions:
;   - The macro has to be executed first, during the initialization phase, since it modifies the stack pointer.
;
; Registers modified:
;   X


.MACRO cpu_init
.SCOPE

; Initialize
    LDX             #stack_s                                          ; Load stack size
    TXS                                                               ; Initialize SP
    SEI                                                               ; Disable interrupts
    CLD                                                               ; Turn off decimal mode

.ENDSCOPE
.ENDMACRO

; ======================================================================================================================

; Idle the CPU in an endless empty loop.


.MACRO idle_loop
.SCOPE

; Idle loop
loop:
    JMP             loop

.ENDSCOPE
.ENDMACRO
