; Libraries for 'test_via_lcd.asm'.

; ======================================================================================================================

; Folders
    .DEFINE         lib_dir "../lib/"                                 ; Parent folder
    .DEFINE         lib_par "par/"                                    ; Parameters folder
    .DEFINE         lib_tab "tab/"                                    ; Tables folder
    .DEFINE         lib_mcr "mcr/"                                    ; Macros folder
    .DEFINE         lib_sub "sub/"                                    ; Subroutines folder

; ======================================================================================================================

; Parameters
    .INCLUDE        .CONCAT(lib_dir, lib_par, "compile_par.inc")      ; Compiler parameters
    .INCLUDE        .CONCAT(lib_dir, lib_par, "hw_par.inc")           ; Hardware parameters

; ======================================================================================================================

; Tables
    .INCLUDE        .CONCAT(lib_dir, lib_tab, "lcdchar_tab.inc")      ; Tables

; ======================================================================================================================

; Macros
    .INCLUDE        .CONCAT(lib_dir, lib_mcr, "cpu_mcr.inc")          ; CPU macros
    .INCLUDE        .CONCAT(lib_dir, lib_mcr, "via_mcr.inc")          ; VIA macros
    .INCLUDE        .CONCAT(lib_dir, lib_mcr, "lcd_mcr.inc")          ; LCD macros
    .INCLUDE        .CONCAT(lib_dir, lib_mcr, "ram_mcr.inc")          ; RAM macros
    .INCLUDE        .CONCAT(lib_dir, lib_mcr, "debug_mcr.inc")        ; Debug macros

; ======================================================================================================================

; Subroutines
    .INCLUDE        .CONCAT(lib_dir, lib_sub, "cpu_sub.inc")          ; CPU subroutines
    .INCLUDE        .CONCAT(lib_dir, lib_sub, "lcd_sub.inc")          ; LCD subroutines
    .INCLUDE        .CONCAT(lib_dir, lib_sub, "math_sub.inc")         ; Mathematical subroutines
    .INCLUDE        .CONCAT(lib_dir, lib_sub, "utils_sub.inc")        ; Utility subroutines
    .INCLUDE        .CONCAT(lib_dir, lib_sub, "txt_sub.inc")          ; Text subroutines
    .INCLUDE        .CONCAT(lib_dir, lib_sub, "debug_sub.inc")        ; Debug subroutines
