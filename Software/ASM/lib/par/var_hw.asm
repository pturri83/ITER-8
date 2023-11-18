; Hardware parameters for the ITER-8.

; ==========================================================

; Clock

    CLK_HZ          = 1000000                               ; Frequency (Hz)

; ==========================================================

; CPU

    CPU_NMIB        = $FFFA                                 ; Non-maskable interrupt vector address
    CPU_RESB        = $FFFC                                 ; Reset vector address
    CPU_IRQB        = $FFFE                                 ; Interrupt request vector address
    CPU_SP          = $FF                                   ; Stack pointer maximum address

; ==========================================================

; ROM

    ROM_A           = $8000                                 ; Base address
    ROM_S           = $8000                                 ; Size

; ==========================================================

; RAM

    RAM_A           = $0000                                 ; Base address
    RAM_S           = $4000                                 ; Size
    RAM_STACK_A     = $0100                                 ; Stack base address
    RAM_STACK_S     = $0100                                 ; Stack size

; ==========================================================

; VIA

    VIA_RB          = $6000                                 ; ORB/IRB address
    VIA_RA          = $6001                                 ; ORA/IRA address
    VIA_DDRB        = $6002                                 ; DDRB address
    VIA_DDRA        = $6003                                 ; DDRA address
    VIA_PCR         = $600C                                 ; PCR address
    VIA_IFR         = $600D                                 ; IFR address
    VIA_IER         = $600E                                 ; IER address

; ==========================================================

; LCD

    LCD_E           = %10000000                             ; Enable signal sent by the VIA port A
    LCD_RWB         = %01000000                             ; R/W signal sent by the VIA port A
    LCD_RS          = %00100000                             ; Register select signal sent by the VIA port A
