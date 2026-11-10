; VIA macros for the ITER-8.

; ======================================================================================================================

; Initialize the VIA.
;
; Registers modified:
;   A


.MACRO via_init

; Prepare adresses
    via_ra_a        = via_a + via_ra                                  ; ORA/IRA adress
    via_rb_a        = via_a + via_rb                                  ; ORB/IRB adress
    via_ddrb_a      = via_a + via_ddrb                                ; DDRB adress
    via_ier_a       = via_a + via_ier                                 ; IER adress
    via_pcr_a       = via_a + via_pcr                                 ; PCR adress

.SCOPE

; Set VIA register B direction
    LDA             #via_outb                                         ; Default data direction of port B
    STA             via_ddrb_a

; Set interrupt pins
    LDA             #via_ie
    ORA             #%1000000                                         ; Enable interrupt pins
    STA             via_ier_a
    LDA             #via_pc                                           ; Set interrupt pins control mode
    STA             via_pcr_a

.ENDSCOPE
.ENDMACRO

; ======================================================================================================================

; Clear an interrupt.
; Port A is read, but the contant is not stored in a register.


.MACRO via_clear
.SCOPE

; Clear port A
    BIT             via_rb_a                                          ; Read port A without storing it

.ENDSCOPE
.ENDMACRO
