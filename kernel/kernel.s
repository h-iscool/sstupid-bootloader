;handle kernel initalization things
[bits 32]

global kernel_init
extern vga_console
extern interrupt_descriptor_table
;;end headers


kernel_init:

;lidt [interrupt_descriptor_table]

call vga_console

hlt
