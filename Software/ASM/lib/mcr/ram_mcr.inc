; RAM macros for the ITER-8.

; ======================================================================================================================

; Initialize the RAM, by clearing it and dumping ROM data into it.
;
; Instructions:
;   - The macro has to be executed right after CPU initialization, since it clears the RAM.
;
; Functions required:
;   clear_ram
;   dumprom_ram
;
; Zero page pointers:
;   ptr_rom
;   ptr_ram_1
;   ptr_ram_2
;
; Registers modified:
;   A, X, Y


.MACRO ram_init
.SCOPE

; Initialize
    clear_ram                                                         ; Clear RAM
    dump_rom_ram                                                      ; Copy data from ROM to RAM

.ENDSCOPE
.ENDMACRO

; ======================================================================================================================

; Clear the RAM.
; The RAM is cleared by overwriting $00 on every byte in RAM_1 (DATA and BSS) that will be used.
;
; Instructions:
;   - The macro has to be executed right after CPU initialization, since it clears the RAM.
;
; Zero page pointers:
;   ptr_ram_1
;
; Registers modified:
;   A, X, Y


.MACRO clear_ram
.SCOPE

; Import symbols
    .IMPORT         __DATA_RUN__, __BSS_RUN__, __BSS_SIZE__

; Prepare counters
    LDA             #.HIBYTE(__DATA_RUN__)
    STA             ptr_ram_1 + 1                                     ; Store high byte of first adress to clear
    LDA             #.LOBYTE(__DATA_RUN__)
    STA             ptr_ram_1                                         ; Store low byte of first adress to clear
    LDA             #$00                                              ; Value to overwrite with

; Clear full pages
    LDY             #0                                                ; Initialize counter
    LDX             #.HIBYTE(__BSS_RUN__ + __BSS_SIZE__ - $0200)      ; Load number of full pages to clear
    BEQ             part                                              ; Skip if there is only a partial page
full:
    STA             (ptr_ram_1), Y                                    ; Clear byte
    INY                                                               ; Next byte
    BNE             full                                              ; Loop back until whole page is done
    INC             ptr_ram_1 + 1                                     ; Next page
    DEX                                                               ; Decrease number of pages
    BNE             full                                              ; Loop back until all full pages are cleared

; Clear partial page
part:
    CPY             #.LOBYTE(__BSS_SIZE__)
    BEQ             exit                                              ; Exit if there are no more bytes to clear
    STA             ($00), Y                                          ; Clear byte
    INY                                                               ; Next byte
    BNE             part                                              ; Loop back until the page is done
exit:

.ENDSCOPE
.ENDMACRO

; ======================================================================================================================

; Copy the DATA segment from ROM to RAM.
;
; Zero page pointers:
;   ptr_rom
;   ptr_ram_1
;   ptr_ram_2
;
; Registers modified:
;   A, Y


.MACRO dump_rom_ram
.SCOPE

; Import symbols
    .IMPORT         __DATA_LOAD__, __DATA_RUN__, __DATA_SIZE__

; Prepare counters
    LDA             #.LOBYTE(__DATA_LOAD__)
    STA             ptr_rom                                           ; Store low byte of start of source in ROM
    LDA             #.HIBYTE(__DATA_LOAD__)
    STA             ptr_rom + 1                                       ; Store high byte of start of source in ROM
    LDA             #.LOBYTE(__DATA_RUN__)
    STA             ptr_ram_1                                         ; Store low byte of start of target in RAM
    LDA             #.HIBYTE(__DATA_RUN__)
    STA             ptr_ram_1 + 1                                     ; Store high byte of start of target in RAM
    LDA             #.LOBYTE(.BITNOT( __DATA_SIZE__))
    STA             ptr_ram_2                                         ; Store low byte of inverse data counter
    LDA             #.HIBYTE(.BITNOT( __DATA_SIZE__))
    STA             ptr_ram_2 + 1                                     ; Store high byte of inverse data counter
    LDY             #0                                                ; Initialize data counter

; Copy data
small_loop:
    INC             ptr_ram_2                                         ; Increase low byte of inverse data counter
    BEQ             skip                                              ; Skip if page is completed
large_loop:
    LDA             (ptr_rom), Y                                      ; Copy data from ROM
    STA             (ptr_ram_1), Y                                    ; Copy data to RAM
    INY                                                               ; Increase data counter
    BNE             small_loop                                        ; Loop if data counter does not reset
    INC             ptr_rom + 1                                       ; Increase page of source in ROM
    INC             ptr_ram_1 + 1                                     ; Increase page of target in RAM
    JMP             small_loop                                        ; Loop back
skip:
    INC             ptr_ram_2 + 1                                     ; Increase high byte of inverse data counter
    BNE             large_loop                                        ; Loop if there are pages left to copy

.ENDSCOPE
.ENDMACRO
