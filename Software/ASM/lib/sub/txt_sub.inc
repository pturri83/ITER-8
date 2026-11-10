; Text subroutines for the ITER-8.

; ======================================================================================================================

; Convert a sequence of bytes in base 10 to an ASCII string. The string ends in 'NULL'.
;
;   - The input sequence has to be stored in '*1' and it has to end in $00.
;   - The starting adress of '*1' has to be loaded in 'ptr_seq'.
;   - The output string has to be stored in '*2'.
;   - The starting adress of '*2' has to be loaded in 'ptr_str'.
;
; Zero page pointers:
;   ptr_seq
;   ptr_str
;
; Input memory:
;   *1 (n B)        bytes sequence
;
; Output memory:
;   *2 (n B)        string
;
; Registers modified:
;   A, Y


convert_10_ascii:
.SCOPE


; Prepare first byte
    LDA             (ptr_seq)                                         ; Load first byte from sequence
    LDY             #0                                                ; Initialize byte counter
    offset          = $30                                             ; ASCII offset between byte '0' and character '0'

; Loop sequence
loop:
    ADC             #offset                                           ; Add ASCII offset to byte
    STA             (ptr_str), Y                                      ; Store character
    INY                                                               ; Increase data counter
    LDA             (ptr_seq), Y                                      ; Load byte from sequence
    BNE             loop                                              ; If the byte is not $00, loop back
    RTS

.ENDSCOPE
