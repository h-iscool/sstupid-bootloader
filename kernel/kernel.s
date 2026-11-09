;handle kernel initalization things
[bits 32]

global kernel_init
extern kernel_init_stage_2
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
push dword [esp+16] ; arg 1 (the success bit) (i copy pasted and was to lazy to delete "the" lol)

call kernel_init_stage_2

hlt
