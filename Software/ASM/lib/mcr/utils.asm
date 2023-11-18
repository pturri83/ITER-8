; Utility macros for the ITER-8.

; ==========================================================

; Add a byte to the beginning of a contiguous sequence of bytes ending in $0000.
;
; Arguments:
; SEQ: starting address of sequence of bytes ending in $0000 (I/O) (1- B)
; BYT: new byte to add (I)
;
; Registers modified:
; A, X, Y

    .MACRO          ADD_SEQ SEQ, BYT
    .SCOPE

; Prepare data
    LDA             BYT
    PHA

; Loop bytes
    LDY             #0                                      ; Initialize counter
LOOP:
    LDA             SEQ, Y                                  ; Load byte from sequence
    TAX
    PLA
    STA             SEQ, Y                                  ; Save byte to sequence
    INY                                                     ; Increase counter
    TXA
    PHA
    BNE             LOOP                                    ; If previous byte is not $0000, loop back
    PLA
    STA             SEQ, Y                                  ; Save $0000 to sequence

    .ENDSCOPE
    .ENDMACRO
