; Script to create a ROM image for the ITER-8.
; Calculates and displays on the LCD a 16-bit division with remainder.
;
; PowerShell scipt: 'test_2.ps1'
; Otput ROM: 'test_2.bin'

; ==========================================================

; Instruction set

    .PC02                                                   ; 65C02

; Parameters

    .INCLUDE        "../lib/par/var_hw.asm"                 ; Hardware parameters
    .INCLUDE        "../lib/par/char_map.asm"               ; Characters map

; Macros

    .INCLUDE        "../lib/mcr/cpu.asm"                    ; CPU functions
    .INCLUDE        "../lib/mcr/lcd.asm"                    ; LCD
    .INCLUDE        "../lib/mcr/utils.asm"                  ; Utilities
    .INCLUDE        "../lib/mcr/math_arithm.asm"            ; Arithmetic

; ==========================================================

; Data

    DIVID_IN        = 5483                                  ; Dividend (16-bit)
    DIVIS_IN        = 59                                    ; Divisor (16-bit)

; RAM adresses

    DIVID           = $0200                                 ; Dividend (2 B)
    DIVIS           = $0202                                 ; Divisor (2 B)
    QUOT            = $0204                                 ; Quotient (2 B)
    REMAIN          = $0206                                 ; Remainder (2 B)
    QUOT_TMP        = $0208                                 ; Temporary quotient (2 B)
    REMAIN_TMP      = $020A                                 ; Temporary remainder (2 B)
    DIVID_STR       = $0210                                 ; Dividend string (6 B)
    DIVIS_STR       = $0216                                 ; Divisor string (6 B)
    QUOT_STR        = $021C                                 ; Quotient string (6 B)
    REMAIN_STR      = $0222                                 ; Remainder string (6 B)

; ==========================================================

; Reset vector

    .SEGMENT        "RESET"
    .WORD           ROM_A

; ==========================================================

; Main code

    .CODE

START:

; Setup hardware
    CPU_SETUP                                               ; CPU
    LCD_SETUP                                               ; LCD

; Dividend
    LDA             DIVID_N                                 ; Store dividend
    STA             DIVID
    LDA             DIVID_N + 1
    STA             DIVID + 1
    BASE10_16       DIVID, DIVID_STR, QUOT_TMP, REMAIN_TMP  ; Convert to base 10

; Divisor
    LDA             DIVIS_N                                 ; Store divisor
    STA             DIVIS
    LDA             DIVIS_N + 1
    STA             DIVIS + 1
    BASE10_16       DIVIS, DIVIS_STR, QUOT_TMP, REMAIN_TMP  ; Convert to base 10

; Division
    DIVIDE_16       DIVID, DIVIS, QUOT, REMAIN

; Quotient
    BASE10_16       QUOT, QUOT_STR, QUOT_TMP, REMAIN_TMP    ; Convert to base 10

; Remainder
    BASE10_16       REMAIN, REMAIN_STR, QUOT_TMP, REMAIN_TMP; Convert to base 10

; Display division
    LCD_PRINT       DIVID_STR
    LCD_CHAR        CHAR_SPACE
    LCD_CHAR        CHAR_DIVISION
    LCD_CHAR        CHAR_SPACE
    LCD_PRINT       DIVIS_STR
    LCD_CHAR        CHAR_SPACE
    LCD_CHAR        CHAR_EQUAL
    JSR             LCD_LINE2
    LCD_PRINT       QUOT_STR
    LCD_CHAR        CHAR_SPACE
    LCD_CHAR        CHAR_R_UP
    LCD_CHAR        CHAR_SPACE
    LCD_PRINT       REMAIN_STR

; Idle
    JSR             IDLE_LOOP

; ==========================================================

; Subroutines

    .INCLUDE        "../lib/sub/lcd.asm"                    ; LCD
    .INCLUDE        "../lib/sub/utils.asm"                  ; Utilities

; ==========================================================

; Data

    .DATA

DIVID_N:
    .LOBYTES        DIVID_IN
    .HIBYTES        DIVID_IN

DIVIS_N:
    .LOBYTES        DIVIS_IN
    .HIBYTES        DIVIS_IN
