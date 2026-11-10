; ITER-8 hardware parameters.

; ======================================================================================================================

; Clock
    clk_hz          = 1000000                                         ; Clock frequency (Hz)
    stack_s         = $FF                                             ; Maximum stack value

; ======================================================================================================================

; VIA
    via_a           = $6000                                           ; Base adress
    via_ra          = %0001                                           ; ORA/IRA register select
    via_rb          = %0000                                           ; ORB/IRB register select
    via_ddrb        = %0010                                           ; DDRB register select
    via_pcr         = $000C                                           ; Peripheral control register select
    via_ifr         = $000D                                           ; Interrupt flag register select
    via_ier         = $000E                                           ; Interrupt enable register select
    via_outb        = %11111110                                       ; Port B pins used as output by default
    via_lcd         = %11111110                                       ; Port B pins used for the LCD
    via_ie          = %00000010                                       ; Interrupt pins enabled
    via_pc          = %00000000                                       ; Interrupt pins control

; ======================================================================================================================

; LCD
    lcd_rs          = %00000010                                       ; Register select signal
    lcd_rwb         = %00000100                                       ; R/W signal
    lcd_e           = %00001000                                       ; Enable signal
