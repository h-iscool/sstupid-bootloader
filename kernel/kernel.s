;handle kernel initalization things
[bits 32]

global kernel_init
extern vga_console
;;end headers


kernel_init:

;load interrupt descriptor tables

call vga_console

hlt

interrupt_descriptor_table:
;db [malloc]
