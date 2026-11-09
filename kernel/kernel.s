;handle kernel initalization things
[bits 32]

global kernel_init
extern kernel_init_stage_2
extern interrupt_descriptor_table
;;end headers


kernel_init:

;lidt [interrupt_descriptor_table]

push dword [esp+16] ; arg 4 (the lfb ptr)
push dword [esp+16] ; arg 3 (the height)
push dword [esp+16] ; arg 2 (the width)
push dword [esp+16] ; arg 1 (the success bit) (i copy pasted and was to lazy to delete "the" lol)


call kernel_init_stage_2

hlt
