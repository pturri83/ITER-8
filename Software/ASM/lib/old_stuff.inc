



; ======================================================================================================================

; Division with remainder in 16-bit.
; The divisor can either be a 2 B adress or a 1 B immediate value.
;
; Input arguments:
; DIVID: initial adress of the dividend (I) (2 B)
; DIVIS: initial adress of the divisor (I) (2 B); immediate value of the divisor (1 B)
; QUOT: initial adress of the quotient (O) (2 B)
; REMAIN: initial adress of the remainder (O) (2 B)
;
; Registers modified:
;   A, X, Y
;
; Requirements:

.MACRO          divide_16 divid, divis, quot, remain
.SCOPE

; Store dividend
    LDA             divid
    PHA
    LDA             divid + 1
    PHA

; Initialize to 0 the remainder and carry bit
    LDA             #0
    STA             remain
    STA             remain + 1
    CLC

; Subtraction loops
    LDX             #16                                               ; Number of bits
loop:

; Rotate left dividend and remainder
    ROL             divid
    ROL             divid + 1
    ROL             remain
    ROL             remain + 1

; Subtract divisor from rotated remainder
    SEC                                                               ; Set carry bit
    LDA             remain

    .IF             .MATCH (.LEFT (1, {divis}), #)                  ; If the divisor argument is an immediate value
    SBC             #<(.RIGHT(.TCOUNT({divis}) - 1, {divis}))       ; Subtract the low byte of the divisor   ; try to substitute < with lobyte. if it still works, use .lobyte
    .ELSE                                                             ; If the divisor argument is an adress
    SBC             divis                                             ; Subtract the low byte of the divisor
    .ENDIF

    TAY                                                               ; Store the subtraction of the low byte
    LDA             remain + 1

    .IF             .MATCH(.LEFT(1, {divis}), #)                  ; If the divisor argument is an immediate value
    SBC             #0                                                ; Subtract 0 because the divisor is 8-bit
    .ELSE                                                             ; If the divisor argument is an adress
    SBC             divis + 1                                         ; Subtract the high byte of the divisor
    .ENDIF

    BCC             skip                                              ; If the subtraction had to borrow, skip
    STY             remain                                            ; Save the low byte of the remainder
    STA             remain + 1                                        ; Save the high byte of the remainder
skip:
    DEX                                                               ; Decrease number of bits left
    BNE             loop                                              ; Loop back

; Prepare output
    ROL             divid                                             ; Rotate the low byte of the dividend once more
    ROL             divid + 1                                         ; Rotate the high byte of the dividend once more
    LDA             divid
    STA             quot                                              ; Store the quotient
    LDA             divid + 1
    STA             quot + 1
    CLC                                                               ; Clear carry bit    ; try to remove it and see if it still works

; Restore dividend         ;if i transform this into a subroutine, maybe i don't need to store and restore the dividend
    PLA
    STA             divid + 1
    PLA
    STA             divid

.ENDSCOPE
.ENDMACRO






; ======================================================================================================================

; Multiplication in 8 bits.
; The multiplicand is loaded in A, the multiplier is stored at a zeropage adress loaded in X.
;
; Input registers:
;   A: multiplicand
;   X: multiplier
;
; Output registers:
;   A: product
;
; Output flags:
;   N, Z, C, V
;
; Registers modified:
;   A

; multipl_8:
; .SCOPE

; ; Multiplication
;     LDA #0       ;Initialize RESULT to 0
;     LDX #8       ;There are 8 bits in NUM2
; L1
;     LSR NUM2     ;Get low bit of NUM2
;     BCC L2       ;0 or 1?
;     CLC          ;If 1, add NUM1
;     ADC NUM1
; L2
;     ROR A        ;"Stairstep" shift (catching carry from add)
;     ROR RESULT
;     DEX
;     BNE L1
;     STA RESULT+1

; .ENDSCOPE

; ======================================================================================================================



; ======================================================================================================================

; Convert a 16-bit binary number into a string in base 10.       ;clean it and make it into a subroutine
;
; Input arguments:
; B2:  initial adress of the binary number (I) (2 B)
; B10:  initial adress of the base-10 string ending in NULL (O) (2-6 B)
; QUOT: initial adress of the temporary quotient (T) (2 B)
; REMAIN: initial adress of the temporary remainder (T) (2 B)
;
; Registers modified:
;   A, X, Y
;
; Requirements:
;

.MACRO          base10_16 B2, B10, QUOT, REMAIN
.SCOPE

; Prepare string
    LDA             #0                                                ; Put NULL in string
    STA             B10

; Save input
    LDA             B2
    PHA
    LDA             B2 + 1
    PHA

; Loop digits
LOOP:
    divide_16       B2, #10, QUOT, REMAIN                             ; Divide by 10
    LDA             REMAIN
    ADC             char_0                                           ; Add remainder to the "0" ASCII value
    STA             REMAIN
    ADD_SEQ         B10, REMAIN                                       ; Add digit to string
    LDA             QUOT + 1                                          ; Save quotient
    STA             B2 + 1
    LDA             QUOT
    STA             B2
    ORA             QUOT + 1                                          ; Check if quotient is zero
    BNE             RANGE                                             ; If it's not the last digit, loop back
    JMP             EXIT
RANGE:                                                                ; Long range jump
    JMP             LOOP

; Restore input
EXIT:
    PLA
    STA             B2 + 1
    PLA
    STA             B2

.ENDSCOPE
.ENDMACRO
