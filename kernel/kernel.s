;handle kernel initalization things
[bits 32]

global kernel_init
extern console
extern vbe_init
extern initScreen

extern interrupt_descriptor_table
extern _init_array_start
extern _init_array_end

;;end headers


kernel_init:



;;prep for C++:

mov edi, _init_array_start ;;constructor loop for making it workssssssssssss
.cotr_loop:
cmp edi, _init_array_end
je .cotr_loop_end
call [edi]
add edi, 4
jmp .cotr_loop
.cotr_loop_end:

;;push vars to C

push dword [esp+16] ; arg 5 (bytes per scan line)
push dword [esp+16] ; arg 4 (the lfb ptr)
push dword [esp+16] ; arg 3 (the height)
push dword [esp+16] ; arg 2 (the width)

cmp dword [esp+16],0x1 ;(the success bit) (i copy pasted and was to lazy to delete "the" lol)
jne vga
call vbe_init ;(vbe_screen_width, vbe_screen_height, bytes_per_scan_line, lfb_pointer);
push 0x2
jmp vga_skip
vga:
push 0x1
vga_skip:

call initScreen
call console

hlt
