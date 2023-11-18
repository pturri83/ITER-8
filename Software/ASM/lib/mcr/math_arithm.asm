; Arithmetic macros for the ITER-8.

; ==========================================================

; Division with remainder in 8-bit.
; The divisor can either be a 1 B address or a 1 B immediate value.
;
; Arguments:
; DIVID: address of the dividend (I) (1 B)
; DIVIS: address of the divisor (I) (1 B); immediate value of the divisor (1 B)
; QUOT: address of the quotient (O) (1 B)
; REMAIN: address of the remainder (O) (1 B)
;
; Registers modified:
; A, X

    .MACRO          DIVIDE_8 DIVID, DIVIS, QUOT, REMAIN
    .SCOPE

; Store dividend
    LDA             DIVID
    PHA

; Initialize quotient, remainder, and carry bit to 0
    LDA             #0
    STA             REMAIN
    CLC

; Subtraction loops
    LDX             #8                                      ; Number of bits
LOOP:

; Rotate left dividend and remainder
    ROL             DIVID
    ROL             REMAIN

; Subtract divisor from rotated remainder
    SEC                                                     ; Set carry bit to 1
    LDA             REMAIN
    .IF             (.MATCH (.LEFT (1, {DIVIS}), #))        ; Subtract divisor
    SBC             #<(.RIGHT (.TCOUNT ({DIVIS})-1, {DIVIS}))
    .ELSE
    SBC             DIVIS
    .ENDIF
    BCC             SKIP                                    ; If subtraction does not succeed, skip
    STA             REMAIN                                  ; Save remainder
SKIP:
    DEX                                                     ; Decrease counter
    BNE             LOOP                                    ; Loop back

; Prepare output
    ROL             DIVID                                   ; Rotate left dividend once more
    LDA             DIVID                                   ; Save quotient
    STA             QUOT
    CLC                                                     ; Clear carry bit

; Restore dividend
    PLA
    STA             DIVID

    .ENDSCOPE
    .ENDMACRO

; ==========================================================

; Division with remainder in 16-bit.
; The divisor can either be a 2 B address or a 1 B immediate value.
;
; Arguments:
; DIVID: initial address of the dividend (I) (2 B)
; DIVIS: initial address of the divisor (I) (2 B); immediate value of the divisor (1 B)
; QUOT: initial address of the quotient (O) (2 B)
; REMAIN: initial address of the remainder (O) (2 B)
;
; Registers modified:
; A, X, Y

    .MACRO          DIVIDE_16 DIVID, DIVIS, QUOT, REMAIN
    .SCOPE

; Store dividend
    LDA             DIVID
    PHA
    LDA             DIVID + 1
    PHA

; Initialize quotient, remainder, and carry bit to 0
    LDA             #0
    STA             REMAIN
    STA             REMAIN + 1
    CLC

; Subtraction loops
    LDX             #16                                     ; Number of bits
LOOP:

; Rotate left dividend and remainder
    ROL             DIVID
    ROL             DIVID + 1
    ROL             REMAIN
    ROL             REMAIN + 1

; Subtract divisor from rotated remainder
    SEC                                                     ; Set carry bit to 1
    LDA             REMAIN
    .IF             (.MATCH (.LEFT (1, {DIVIS}), #))        ; Subtract first byte of divisor
    SBC             #<(.RIGHT (.TCOUNT ({DIVIS})-1, {DIVIS}))
    .ELSE
    SBC             DIVIS
    .ENDIF
    TAY
    LDA             REMAIN + 1
    .IF             (.MATCH (.LEFT (1, {DIVIS}), #))        ; Subtract second byte of divisor
    SBC             #0
    .ELSE
    SBC             DIVIS + 1
    .ENDIF
    BCC             SKIP                                    ; If subtraction does not succeed, skip
    STY             REMAIN                                  ; Save first byte of remainder
    STA             REMAIN + 1                              ; Save second byte of remainder
SKIP:
    DEX                                                     ; Decrease counter
    BNE             LOOP                                    ; Loop back

; Prepare output
    ROL             DIVID                                   ; Rotate left dividend once more
    ROL             DIVID + 1
    LDA             DIVID                                   ; Save quotient
    STA             QUOT
    LDA             DIVID + 1
    STA             QUOT + 1
    CLC                                                     ; Clear carry bit

; Restore dividend
    PLA
    STA             DIVID + 1
    PLA
    STA             DIVID

    .ENDSCOPE
    .ENDMACRO
