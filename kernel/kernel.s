;handle kernel initalization things
[bits 32]

section .bss
align 16
stack_bottom:
resb 16384 ;; 16kb stack
stack_top:

section .text

global kernel_init
extern console
extern vbe_init
extern initScreen

extern LOAD_TESTING ;; just... yeah | There is a enable_testing flag in stage2.asm , its useful for testing stuff in C, CPP or asm
extern call_load_testing

extern interrupt_descriptor_table
extern _init_array_start
extern _init_array_end

;;end headers


kernel_init:

mov esp, stack_top



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

; push dword [esp+16] ; arg 5 (bytes per scan line) //
; push dword [esp+16] ; arg 4 (the lfb ptr)
; push dword [esp+16] ; arg 3 (the height)
; push dword [esp+16] ; arg 2 (the width)

; cmp dword [esp+16],0x1 ;(the success bit) (i copy pasted and was to lazy to delete "the" lol)
; jne vga
; call vbe_init ;(vbe_screen_width, vbe_screen_height, bytes_per_scan_line, lfb_pointer);
; push 0x2
; jmp vga_skip
; vga:
push 0x1
; vga_skip:

extern idt_init ;; load idt init from c
call idt_init

extern int_remaps
call int_remaps

;; initialize timer thing so 1 PIT call is 1ms 
;; idk if this should be in c code or not but here it is going to be for now:
extern time_init
call time_init

call initScreen ;; load after idt so any errors can use the screen (LATER ADD SWITCH INSIDE OF THE SCREEN.C FILE)



sti ;;enable ints



;;testing call if enabled:
cmp byte [call_load_testing], 0x0
je .skip_testing_call
call LOAD_TESTING
.skip_testing_call:



call console

cli ;disable interrupts so we cant recover
hlt