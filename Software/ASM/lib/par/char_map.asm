; Character map for the LCD-016M002B screen of the ITER-8.

; ==========================================================

; Numbers

    CHAR_0          = %00110000                             ; "0"
    CHAR_1          = %00110001                             ; "1"
    CHAR_2          = %00110010                             ; "2"
    CHAR_3          = %00110011                             ; "3"
    CHAR_4          = %00110100                             ; "4"
    CHAR_5          = %00110101                             ; "5"
    CHAR_6          = %00110110                             ; "6"
    CHAR_7          = %00110111                             ; "7"
    CHAR_8          = %00111000                             ; "8"
    CHAR_9          = %00111001                             ; "9"

; ==========================================================

; Lower case letters

    CHAR_A          = %01100001                             ; "a"
    CHAR_B          = %01100010                             ; "b"
    CHAR_C          = %01100011                             ; "c"
    CHAR_D          = %01100100                             ; "d"
    CHAR_E          = %01100101                             ; "e"
    CHAR_F          = %01100110                             ; "f"
    CHAR_G          = %01100111                             ; "g"
    CHAR_H          = %01101000                             ; "h"
    CHAR_I          = %01101001                             ; "i"
    CHAR_J          = %01101010                             ; "j"
    CHAR_K          = %01101011                             ; "k"
    CHAR_L          = %01101100                             ; "l"
    CHAR_M          = %01101101                             ; "m"
    CHAR_N          = %01101110                             ; "n"
    CHAR_O          = %01101111                             ; "o"
    CHAR_P          = %01110000                             ; "p"
    CHAR_Q          = %01110001                             ; "q"
    CHAR_R          = %01110010                             ; "r"
    CHAR_S          = %01110011                             ; "s"
    CHAR_T          = %01110100                             ; "t"
    CHAR_U          = %01110101                             ; "u"
    CHAR_V          = %01110110                             ; "v"
    CHAR_W          = %01110111                             ; "w"
    CHAR_X          = %01111000                             ; "x"
    CHAR_Y          = %01111001                             ; "y"
    CHAR_Z          = %01111010                             ; "z"

; ==========================================================

; Upper case letters

    CHAR_A_UP       = %01000001                             ; "A"
    CHAR_B_UP       = %01000010                             ; "B"
    CHAR_C_UP       = %01000011                             ; "C"
    CHAR_D_UP       = %01000100                             ; "D"
    CHAR_E_UP       = %01000101                             ; "E"
    CHAR_F_UP       = %01000110                             ; "F"
    CHAR_G_UP       = %01000111                             ; "G"
    CHAR_H_UP       = %01001000                             ; "H"
    CHAR_I_UP       = %01001001                             ; "I"
    CHAR_J_UP       = %01001010                             ; "J"
    CHAR_K_UP       = %01001011                             ; "K"
    CHAR_L_UP       = %01001100                             ; "L"
    CHAR_M_UP       = %01001101                             ; "M"
    CHAR_N_UP       = %01001110                             ; "N"
    CHAR_O_UP       = %01001111                             ; "O"
    CHAR_P_UP       = %01010000                             ; "P"
    CHAR_Q_UP       = %01010001                             ; "Q"
    CHAR_R_UP       = %01010010                             ; "R"
    CHAR_S_UP       = %01010011                             ; "S"
    CHAR_T_UP       = %01010100                             ; "T"
    CHAR_U_UP       = %01010101                             ; "U"
    CHAR_V_UP       = %01010110                             ; "V"
    CHAR_W_UP       = %01010111                             ; "W"
    CHAR_Y_UP       = %01011001                             ; "Y"
    CHAR_Z_UP       = %01011010                             ; "Z"

; ==========================================================

; Greek letters
    CHAR_ALPHA      = %11100000                             ; Alpha
    CHAR_BETA       = %11100010                             ; Beta
    CHAR_EPSILON    = %11100011                             ; Epsilon
    CHAR_THETA      = %11110010                             ; Theta
    CHAR_MU         = %11100100                             ; Mu
    CHAR_PI         = %11110111                             ; Pi
    CHAR_RHO        = %11100110                             ; Rho
    CHAR_SIGMA      = %11100101                             ; Sigma
    CHAR_SIGMA_UP   = %11110110                             ; Sigma upper case
    CHAR_OMEGA_UP   = %11110100                             ; Omega upper case

; ==========================================================

; Punctuation

    CHAR_SPACE      = %00100000                             ; " "
    CHAR_COMMA      = %00101100                             ; ","
    CHAR_STOP       = %00101110                             ; "."
    CHAR_COLON      = %00111010                             ; ":"
    CHAR_SEMICOLON  = %00111011                             ; ";"
    CHAR_QUESTION   = %00111111                             ; "?"
    CHAR_EXCLAMATION= %00100001                             ; "!"
    CHAR_AMPERSAND  = %00100110                             ; "&"
    CHAR_QUOTE      = %00100010                             ; """
    CHAR_APOSTROPHE = %00100111                             ; "'"
    CHAR_BACKTICK   = %01100000                             ; "`"
    CHAR_DASH       = %00101101                             ; "-"
    CHAR_SLASH      = %00101111                             ; "/"
    CHAR_BAR        = %01111100                             ; "|"
    CHAR_PARENTH_L  = %00101000                             ; "("
    CHAR_PARENTH_R  = %00101001                             ; ")"
    CHAR_BRACKET_L  = %01011011                             ; "["
    CHAR_BRACKET_R  = %01011101                             ; "]"
    CHAR_CURLY_L    = %01111011                             ; "{"
    CHAR_CURLY_R    = %01111101                             ; "}"

; ==========================================================

; Symbols

    CHAR_CHEVRON_L  = %00111100                             ; "<"
    CHAR_CHEVRON_R  = %00111110                             ; ">"
    CHAR_EQUAL      = %00111101                             ; "="
    CHAR_PLUS       = %00101011                             ; "+"
    CHAR_STAR       = %00101010                             ; "*"
    CHAR_DIVISION   = %11111101                             ; Division
    CHAR_PERCENT    = %00100101                             ; "%"
    CHAR_SQRT       = %11101000                             ; Square root
    CHAR_INFINITE   = %11110011                             ; Infinite
    CHAR_DOT        = %10100101                             ; Middle dot
    CHAR_CARET      = %01011110                             ; "^"
    CHAR_UNDERSCORE = %01011111                             ; "_"
    CHAR_HASH       = %00100011                             ; "#"
    CHAR_AT         = %01000000                             ; "@"
    CHAR_DOLLAR     = %00100100                             ; "$"
    CHAR_CENT       = %11101100                             ; Cent
    CHAR_ARROW_L    = %01111111                             ; Left arrow
    CHAR_ARROW_R    = %01111110                             ; Right arrow
    CHAR_BLOCK      = %11111111                             ; Block

; ==========================================================

; Special

    CHAR_NULL       = %00000000                             ; NULL
