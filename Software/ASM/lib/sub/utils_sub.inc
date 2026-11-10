; Utilitity subroutines for the ITER-8.

; ======================================================================================================================

; Insert a byte at the beginning of a sequence of bytes. The sequence ends in $00.
;
; Instructions:
;   - The input sequence has to be stored in '*' and it has to end in at least two $00 bytes. One to allow the sequence
;     to expand, the other to indicate the end of the sequence.
;   - The starting adress of '*' has to be loaded in 'ptr_seq'.
;
; Zero page pointers:
;   ptr_seq
;
; Input registers:
;   A               byte to insert
;
; Input memory:
;   * (n B)         bytes sequence
;
; Output memory:
;   * (n+1 B)       expanded bytes sequence
;
; Registers modified:
;   A, X, Y


insert_byte:
.SCOPE

; Prepare byte
    PHA                                                               ; Store byte to insert
    LDY             #0                                                ; Initialize byte counter

; Loop sequence
loop:
    LDA             (ptr_str), Y                                      ; Load byte from sequence
    TAX                                                               ; Store sequence byte
    PLA                                                               ; Restore byte to insert
    STA             (ptr_str), Y                                      ; Add byte to sequence
    INY                                                               ; Increase byte counter
    TXA                                                               ; Restore sequence byte
    PHA                                                               ; Store sequence byte
    BNE             loop                                              ; If sequence byte is not $00, loop back
    PLA                                                               ; Restore $00
    STA             (ptr_str), Y                                      ; Add $00 to sequence
    RTS

.ENDSCOPE

; ======================================================================================================================

; Convert a 8 bit number into a sequence of bytes in base 10. The sequence is always 4 bytes long, ending in $00.
;
; Instructions:
;   - A temporary calculation has to be stored in '*1'. It does not need to be initialized.
;   - The starting adress of '*1' has to be loaded in 'ptr_div'.
;   - The output sequence has to be stored in '*2'.
;   - The starting adress of '*2' has to be loaded in 'ptr_seq'.
;
; Functions required:
;   divide_8
;   insert_byte
;
; Zero page pointers:
;   ptr_div
;   ptr_seq
;
; Input registers:
;   A               number to convert
;
; Input memory:
;   *1 (4 B)        temporary calculations
;
; Output memory:
;   *2 (4 B)        bytes sequence in base 10
;
; Registers modified:
;   A, X, Y


convert_8_10:
.SCOPE

; Initialize
    STA             (ptr_div)                                         ; Store number
    LDA             #10                                               ; Store base 10
    LDY             #1
    STA             (ptr_div), Y
    LDA             #$00                                              ; Load byte %00
    STA             (ptr_seq)                                         ; Initialize sequence

; Loop digits
loop:
    JSR             divide_8                                          ; Divide number
    LDY             #3                                                ; Store remainder
    LDA             (ptr_div), Y
    JSR             insert_byte                                       ; Add byte to sequence
    LDY             #2                                                ; Load quotient
    LDA             (ptr_div), Y
    BEQ             exit                                              ; If the quotient is zero, exit
    STA             (ptr_div)                                         ; Store quotient as new dividend
    JMP             loop                                              ; Jump back
exit:
    RTS

.ENDSCOPE
