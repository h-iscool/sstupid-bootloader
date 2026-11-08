;handle kernel initalization things
[bits 32]

global kernel_init
extern vga_console
;;end headers


kernel_init:

call vga_console

hlt
