; LCD macros for the ITER-8.

; ======================================================================================================================

; Initialize the LCD.
;
; Instructions:
;   - The macro has to be executed after the VIA initialization, since it has to communicate with it.
;
; Functions required:
;   delay_ms
;   delay_mss
;   lcd_instruct_nowait
;   lcd_instruct
;   lcd_home
;   lcd_clear
;
; Symbols required:
;   debug           debug step-by-step (0 = False, 1 = True)
;
; Input arguments:
;   cursor          cursor mode ("no_cursor", default - "cursor_no_blink" - "cursor_blink")
;   cursor_shift    cursor shift direction ("right", default - "left")
;   display_shift   display shift mode ("no_shift", default - "shift")
;
; Registers modified:
;   A, X, Y


.MACRO lcd_init     cursor, cursor_shift, display_shift
.SCOPE

; Input arguments:
.IFBLANK            cursor
    .DEFINE         cursor_new "no_cursor"
.ELSE
    .DEFINE         cursor_new cursor
.ENDIF

.IFBLANK            cursor_shift
    .DEFINE         cursor_shift_new "right"
.ELSE
    .DEFINE         cursor_shift_new cursor_shift
.ENDIF

.IFBLANK            display_shift
    .DEFINE         display_shift_new "no_shift"
.ELSE
    .DEFINE         display_shift_new display_shift
.ENDIF

; Start LCD
.IF                 ::debug = 0                                       ; If not debugging
    LDY             #50                                               ; Wait 50 ms after power on
    JSR             delay_mss
.ENDIF

    LDA             #%00110000                                        ; Set data length to 8 bits (1/3)
    JSR             lcd_instruct_nowait

.IF                 ::debug = 0                                       ; If not debugging
    LDY             #5                                                ; Wait 5 ms
    JSR             delay_mss
.ENDIF

    LDA             #%00110000                                        ; Set data length to 8 bits (2/3)
    JSR             lcd_instruct_nowait

.IF                 ::debug = 0                                       ; If not debugging
    JSR             delay_ms                                          ; Wait 1 ms
.ENDIF

    LDA             #%00110000                                        ; Set data length to 8 bits (3/3)
    JSR             lcd_instruct_nowait

.IF                 ::debug = 0                                       ; If not debugging
    JSR             delay_ms                                          ; Wait 1 ms
.ENDIF

; Set 4-bit mode, 2 lines, 5x8 fonts
    LDA             #%00100000                                        ; Set 4-bit mode
    JSR             lcd_instruct_nowait
    LDA             #%00101000                                        ; Set 4-bit mode, 2 lines, 5x8 fonts
    JSR             lcd_instruct

; Set cursor
.IF                 .XMATCH(cursor_new, "no_cursor")
    LDA             #%00001100                                        ; Cursor off
.ELSEIF             .XMATCH(cursor_new, "cursor_no_blink")
    LDA             #%00001110                                        ; Cursor on, no blinking
.ELSEIF             .XMATCH(cursor_new, "cursor_blink")
    LDA             #%00001111                                        ; Cursor on, blinking
.ENDIF

    JSR             lcd_instruct

; Set cursor shift and display shift
.IF                 .XMATCH(cursor_shift_new, "left")                 ; Shift the cursor left
    cursor_bit      = %00000000
.ELSEIF             .XMATCH(cursor_shift_new, "right")                ; Shift the cursor right
    cursor_bit      = %00000010
.ENDIF

.IF                 .XMATCH(display_shift_new, "no_shift")            ; No display shift
    display_bit     = %00000000
.ELSEIF             .XMATCH(display_shift_new, "shift")               ; Display shift
    display_bit     = %00000001
.ENDIF

    mode_bit        = %00000100                                       ; Entry mode set
    shift_bits      = mode_bit .BITOR cursor_bit .BITOR display_bit   ; Combine bits
    LDA             #shift_bits
    JSR             lcd_instruct

; Clear LCD
    JSR             lcd_home                                          ; Return cursor home
    JSR             lcd_clear                                         ; Clear display

.ENDSCOPE
.ENDMACRO
