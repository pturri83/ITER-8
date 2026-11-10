; Mathematical subroutines for the ITER-8.

; ======================================================================================================================

; Addition in 8 bits.
; The sum is 16 bits. All values are non-negative and little-endian.
;
; Instructions:
;   - The first and second addend, and sum have to be stored in '*', in this order. The sum does not need to be
;     initialized.
;   - The starting adress of '*' has to be loaded in 'ptr_add'.
;
; Zero page pointers:
;   ptr_add
;
; Input memory:
;   * (4 B)         first addend (1 B), second addend (1 B), - (2 B)
;
; Output memory:
;   * (4 B)         first addend (1 B), second addend (1 B), sum (2 B)
;
; Registers modified:
;   A, Y


add_8:
.SCOPE

; Initialize values
    LDA             #0                                                ; Initial value
    LDY             #3                                                ; Initialize sum high byte
    STA             (ptr_add), Y
    LDA             (ptr_add)                                         ; Load first addend
    CLC                                                               ; Clear carry bit

; Addition
    LDY             #1                                                ; Add second addend
	ADC             (ptr_add), Y
    LDY             #2                                                ; Store sum low byte
    STA             (ptr_add), Y
    BCC             skip                                              ; If no carry bit, skip
    LDA             #1                                                ; Prepare carry bit
    LDY             #3                                                ; Store sum high byte
    STA             (ptr_add), Y
skip:
    RTS

.ENDSCOPE

; ======================================================================================================================

; Addition in 16 bits.
; The sum is 24 bits. All values are non-negative and little-endian.
;
; Instructions:
;   - The first and second addend, and sum have to be stored in '*', in this order. The sum does not need to be
;     initialized.
;   - The starting adress of '*' has to be loaded in 'ptr_add_2'.
;
; Zero page pointers:
;   ptr_add_2
;
; Input memory:
;   * (7 B)         first addend (2 B), second addend (2 B), - (3 B)
;
; Output memory:
;   * (7 B)         first addend (2 B), second addend (2 B), sum (3 B)
;
; Registers modified:
;   A, Y


add_16:
.SCOPE

; Initialize values
    LDA             #0                                                ; Initial value
    LDY             #6                                                ; Initialize sum highest byte
    STA             (ptr_add_2), Y
    LDA             (ptr_add_2)                                       ; Load first addend
    CLC                                                               ; Clear carry bit

; Addition
    LDY             #2                                                ; Add second addend
	ADC             (ptr_add_2), Y
    LDY             #4                                                ; Store sum
    STA             (ptr_add_2), Y
    LDY             #1
    LDA             (ptr_add_2), Y
    LDY             #3
	ADC             (ptr_add_2), Y
    LDY             #5
    STA             (ptr_add_2), Y
    BCC             skip                                              ; If no carry bit, skip
    LDA             #1                                                ; Prepare carry bit
    LDY             #6                                                ; Store sum highest byte
    STA             (ptr_add_2), Y
skip:
    RTS

.ENDSCOPE

; ======================================================================================================================

; Subtraction in 8 bits.
; The difference is 8 bits, with the carry flag. All values are non-negative, except the difference that can use the
; carry bit.
;
; Instructions:
;   - The minuend, subtrahend and difference have to be stored in '*', in this order. The difference does not need to be
;     initialized.
;   - The starting adress of '*' has to be loaded in 'ptr_sub'.
;
; Zero page pointers:
;   ptr_sub
;
; Input memory:
;   * (3 B)         minuend (1 B), subtrahend (1 B), - (1 B)
;
; Output memory:
;   * (3 B)         minuend (1 B), subtrahend (1 B), difference (1 B)
;
; Output flags:
;   C               0 = negative difference; 1 = positive difference
;
; Registers modified:
;   A, Y


sub_8:
.SCOPE

; Initialize values
    LDA             (ptr_sub)                                         ; Load minuend
    SEC                                                               ; Set carry bit

; Subtraction
    LDY             #1                                                ; Subtract subtrahend
	SBC             (ptr_sub), Y
    LDY             #2                                                ; Store difference
    STA             (ptr_sub), Y
    RTS

.ENDSCOPE

; ======================================================================================================================

; Subtraction in 16 bits.
; The difference is 16 bits, with the carry flag. All values are non-negative, except the difference that can use the
; carry bit. All numbers are little-endian.
;
; Instructions:
;   - The minuend, subtrahend and difference have to be stored in '*', in this order. The difference does not need to be
;     initialized.
;   - The starting adress of '*' has to be loaded in 'ptr_sub_2'.
;
; Zero page pointers:
;   ptr_sub_2
;
; Input memory:
;   * (6 B)         minuend (2 B), subtrahend (2 B), - (2 B)
;
; Output memory:
;   * (6 B)         minuend (2 B), subtrahend (2 B), difference (2 B)
;
; Output flags:
;   C               0 = negative difference; 1 = positive difference
;
; Registers modified:
;   A, Y


sub_16:
.SCOPE

; Initialize values
    LDA             (ptr_sub_2)                                       ; Load minuend
    SEC                                                               ; Set carry bit

; Subtraction
    LDY             #2                                                ; Subtract subtrahend
	SBC             (ptr_sub_2), Y
    LDY             #4                                                ; Store difference
    STA             (ptr_add_2), Y
    LDY             #1
    LDA             (ptr_sub_2), Y
    LDY             #3
	SBC             (ptr_sub_2), Y
    LDY             #5
    STA             (ptr_sub_2), Y
    RTS

.ENDSCOPE

; ======================================================================================================================

; Multiplication in 8 bits.
; The product is 16 bits. All values are non-negative and little-endian.
;
; Instructions:
;   - The multiplicand, multiplier and product have to be stored in '*', in this order. The product does not need to be
;     initialized.
;   - The starting adress of '*' has to be loaded in 'ptr_mult'.
;
; Zero page pointers:
;   ptr_mult
;
; Input memory:
;   * (4 B)         multiplicand (1 B), multiplier (1 B), _ (2 B)
;
; Output memory:
;   * (4 B)         multiplicand (1 B), multiplier (1 B), product (2 B)
;
; Registers modified:
;   A, X, Y


mult_8:
.SCOPE

; Initialize values
    LDA             (ptr_mult)                                        ; Load multiplicand
    PHA                                                               ; Store multiplicand
    LDX             #8                                                ; Number of bits

; Addition
    LSR             A                                                 ; Shift multiplicand
    STA             (ptr_mult)                                        ; Store shifted multiplicand
    LDA             #0                                                ; Initial product value
loop:
    BCC             skip                                              ; If no carry bit, skip
    CLC                                                               ; Clear carry bit
    LDY             #1                                                ; Add multiplier
    ADC             (ptr_mult), Y
skip:
    ROR             A                                                 ; Rotate product
    LDY             #3                                                ; Store rotated product high byte
    STA             (ptr_mult), Y
    LDA             (ptr_mult)                                        ; Load rotated multiplicand
    ROR             A                                                 ; Rotate multiplicand
    STA             (ptr_mult)                                        ; Store rotated multiplicand
    DEX                                                               ; Decrease number of bits left
    BNE             loop                                              ; If there are bits left, loop back
    ; LDY             #3                                                ; Store product high byte
    ; STA             (ptr_mult), Y
    LDA             (ptr_mult)                                        ; Load product low byte
    LDY             #2                                                ; Store product low byte
    STA             (ptr_mult), Y

; Restore multiplicand
    PLA                                                               ; Store multiplicand
    STA             (ptr_mult)
    RTS

.ENDSCOPE

; ======================================================================================================================

; Multiplication in 16 bits.
; The product is 32 bits. All values are non-negative and little-endian.
;
; Instructions:
;   - The multiplicand, multiplier and product have to be stored in '*', in this order. The product does not need to be
;     initialized.
;   - The starting adress of '*' has to be loaded in 'ptr_mult_2'.
;
; Zero page pointers:
;   ptr_mult_2
;
; Input memory:
;   * (8 B)         multiplicand (2 B), multiplier (2 B), _ (4 B)
;
; Output memory:
;   * (8 B)         multiplicand (2 B), multiplier (2 B), product (4 B)
;
; Registers modified:
;   A, X, Y


mult_16:
.SCOPE

; Initialize values
    LDA             (ptr_mult)                                        ; Load multiplicand
    PHA                                                               ; Store multiplicand
    LDY             #1
    LDA             (ptr_mult), Y
    PHA
    LDA             (ptr_mult)                                        ; Load multiplicand
    LDX             #16                                               ; Number of bits

; Addition
    LSR             A                                                 ; Shift multiplicand
    STA             (ptr_mult)                                        ; Store shifted multiplicand
    LDA             #0                                                ; Initial product value
loop:
    BCC             skip                                              ; If no carry bit, skip
    CLC                                                               ; Clear carry bit
    LDY             #1                                                ; Add multiplier
    ADC             (ptr_mult), Y
skip:
    ROR             A                                                 ; Rotate product

    LDA             (ptr_mult)                                        ; Load rotated multiplicand
    ROR             A                                                 ; Rotate multiplicand
    STA             (ptr_mult)                                        ; Store rotated multiplicand
    DEX                                                               ; Decrease number of bits left
    BNE             loop                                              ; If there are bits left, loop back
    LDY             #3                                                ; Store product high byte
    STA             (ptr_mult), Y
    LDA             (ptr_mult)                                        ; Load product low byte
    LDY             #2                                                ; Store product low byte
    STA             (ptr_mult), Y

; Restore multiplicand
    PLA                                                               ; Store multiplicand
    STA             (ptr_mult)
    RTS

.ENDSCOPE

; ======================================================================================================================

; Integer division with remainder in 8 bit.             division by 0?
; The quotient and remainder are 8 bits. All values are non-negative.
;
; Instructions:
;   - The dividend, divisor, quotient and remainder have to be stored in '*', in this order. The quotient and remainder
;     do not need to be initialized.
;   - The starting adress of '*' has to be loaded in 'ptr_div'.
;
; Zero page pointers:
;   ptr_div
;
; Input memory:
;   * (4 B)         dividend (1 B), divisor (1 B), - (1 B), - (1 B)
;
; Output memory:
;   * (4 B)         dividend (1 B), divisor (1 B), quotient (1 B), remainder (1 B)
;
; Registers modified:
;   A, X, Y


divide_8:
.SCOPE

; Initialize values
    LDA             (ptr_div)                                         ; Load dividend
    PHA                                                               ; Store dividend
    LDA             #0                                                ; Initial value
    LDY             #3                                                ; Initialize remainder
    STA             (ptr_div), Y
    LDX             #8                                                ; Number of bits
    CLC                                                               ; Clear carry bit
loop:

; Rotate terms
    LDA             (ptr_div)                                         ; Load rotated dividend
    ROL             A                                                 ; Rotate dividend
    STA             (ptr_div)                                         ; Store rotated dividend
    LDY             #3                                                ; Load rotated remainder
    LDA             (ptr_div), Y
    ROL             A                                                 ; Rotate remainder
    STA             (ptr_div), Y                                      ; Store rotated remainder

; Subtraction
    SEC                                                               ; Set carry bit
    LDY             #1                                                ; Subtract divisor from rotated remainder
    SBC             (ptr_div), Y
    BCC             skip                                              ; If subtraction had to borrow, skip
    LDY             #3                                                ; Store subtracted rotated remainder
    STA             (ptr_div), Y
skip:
    DEX                                                               ; Decrease number of bits left
    BNE             loop                                              ; If there are bits left, loop back

; Prepare output
    LDA             (ptr_div)                                         ; Load rotated dividend
    ROL             A                                                 ; Rotate dividend once more
    LDY             #2                                                ; Store quotient
    STA             (ptr_div), Y

; Restore dividend
    PLA                                                               ; Store dividend
    STA             (ptr_div)
    RTS

.ENDSCOPE

; ======================================================================================================================

; Integer division with remainder in 16 bit.
; The quotient and remainder are 16 bits. All values are non-negative and little-endian.
;
; Instructions:
;   - The dividend, divisor, quotient and remainder have to be stored in '*', in this order. The quotient and remainder
;     do not need to be initialized.
;   - The starting adress of '*' has to be loaded in 'ptr_div_2'.
;
; Zero page pointers:
;   ptr_div_2
;
; Input memory:
;   * (8 B)         dividend (2 B), divisor (2 B), - (2 B), - (2 B)
;
; Output memory:
;   * (8 B)         dividend (2 B), divisor (2 B), quotient (2 B), remainder (2 B)
;
; Registers modified:
;   A, X, Y


divide_16:
.SCOPE

; Initialize values
    LDA             (ptr_div)                                         ; Load dividend
    PHA                                                               ; Store dividend
    LDY             #1
    LDA             (ptr_div), Y
    PHA
    LDA             #0                                                ; Initial value
    LDY             #6                                                ; Initialize remainder
    STA             (ptr_div), Y
    LDY             #7
    STA             (ptr_div), Y
    LDX             #16                                               ; Number of bits
    CLC                                                               ; Clear carry bit
loop:

; Rotate terms
    LDA             (ptr_div)                                         ; Load rotated dividend
    ROL             A                                                 ; Rotate dividend
    STA             (ptr_div)                                         ; Store rotated dividend
    LDY             #1
    LDA             (ptr_div), Y
    ROL             A
    STA             (ptr_div), Y
    LDY             #6                                                ; Load rotated remainder
    LDA             (ptr_div), Y
    ROL             A                                                 ; Rotate remainder
    STA             (ptr_div), Y                                      ; Store rotated remainder
    LDY             #7
    LDA             (ptr_div), Y
    ROL             A
    STA             (ptr_div), Y

; Subtraction
    SEC                                                               ; Set carry bit
    LDY             #6                                                ; Load rotated remainder
    LDA             (ptr_div), Y
    LDY             #2                                                ; Subtract divisor from rotated remainder
    SBC             (ptr_div), Y
    LDY             #7
    LDA             (ptr_div), Y
    LDY             #3
    SBC             (ptr_div), Y
    BCC             skip                                              ; If subtraction had to borrow, skip
    LDY             #6                                                ; Store subtracted rotated remainder
    STA             (ptr_div), Y
    LDY             #7
    STA             (ptr_div), Y

skip:
    DEX                                                               ; Decrease number of bits left
    BNE             loop                                              ; If there are bits left, loop back

; Prepare output
    LDA             (ptr_div)                                         ; Load rotated dividend
    ROL             A                                                 ; Rotate dividend once more
    LDY             #4                                                ; Store quotient
    STA             (ptr_div), Y
    LDY             #1
    LDA             (ptr_div), Y
    ROL             A
    LDY             #5
    STA             (ptr_div), Y

; Restore dividend
    PLA                                                               ; Store dividend
    LDY             #1
    STA             (ptr_div), Y
    PLA
    STA             (ptr_div)
    CLC                                                               ; Clear carry bit
    RTS

.ENDSCOPE
