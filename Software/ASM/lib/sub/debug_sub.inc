; Debug subroutines for the ITER-8.

; ======================================================================================================================

; Store the PC register value in X and Y, as it was right before calling this subroutine.   ;check if this is correct or if it needs an offset

debug_pc:
.SCOPE

    TSX             ; Store S
    LDA             $100, X
    TAY
    LDA             $101, X
    TAX

.ENDSCOPE
