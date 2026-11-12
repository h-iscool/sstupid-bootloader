extern _fault_handler

isr_common_stub:
pusha
mov ax, ds
push eax ;;save ds disc

mov ax, 0x10 ;; load kernel ds
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax

push esp ;; give ptr to c
call _fault_handler
add esp, 4

pop eax
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
popa
add esp, 8
iret ;; int return

%macro ISR_NOERRCODE 1
global _isr%1
_isr%1: ;;this is cool asf to define something with a macro, ty google ai for telling me i could optimize like this, my code would be a LOT longer otherwise
cli
push byte 0 ;; err code 0 if no err code
push byte %1
jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
global _isr%1
_isr%1:
cli
;;cpu did the pushbyte alreadt
push byte %1
jmp isr_common_stub
%endmacro

;;macro define ISRS:
ISR_NOERRCODE 0 ; 0: Divide By Zero Exception
ISR_NOERRCODE 1 ; 1: Debug Exception
ISR_NOERRCODE 2 ; 2: Non Maskable Interrupt Exception
ISR_NOERRCODE 3 ; 3: Breakpoint Exception
ISR_NOERRCODE 4 ; 4: Into Detected Overflow Exception
ISR_NOERRCODE 5 ; 5: Out of Bounds Exception
ISR_NOERRCODE 6 ; 6: Invalid Opcode Exception
ISR_NOERRCODE 7 ; 7: No Coprocessor Exception
ISR_ERRCODE 8 ; 8: Double Fault Exception
ISR_NOERRCODE 9 ; 9: Coprocessor Segment Overrun Exception
ISR_ERRCODE 10; 10: Bad TSS Exception
ISR_ERRCODE 11; 11: Segment Not Present Exception
ISR_ERRCODE 12; 12: Stack Fault Exception
ISR_ERRCODE 13; 13: General Protection Fault Exception
ISR_ERRCODE 14; 14: Page Fault Exception
ISR_NOERRCODE 15; 15: Unknown Interrupt Exception (Reserved)
ISR_NOERRCODE 16; 16: Coprocessor Fault (x87 Floating-Point) Exception
ISR_ERRCODE 17; 17: Alignment Check Exception
ISR_NOERRCODE 18; 18: Machine Check Exception
ISR_NOERRCODE 19; 19: SIMD Floating-Point Exception
ISR_NOERRCODE 20; 20: Virtualization Exception
ISR_ERRCODE 21; 21: Control Protection Exception
ISR_NOERRCODE 22; 22: Reserved
ISR_NOERRCODE 23; 23: Reserved
ISR_NOERRCODE 24; 24: Reserved
ISR_NOERRCODE 25; 25: Reserved
ISR_NOERRCODE 26; 26: Reserved
ISR_NOERRCODE 27; 27: Reserved
ISR_NOERRCODE 28; 28: Hypervisor Injection Exception
ISR_ERRCODE 29; 29: VMM Communication Exception
ISR_ERRCODE 30; 30: Security Exception
ISR_NOERRCODE 31; 31: Reserved



;; CUSTOM ISRS:

extern timer_tick_call
global _isr32
_isr32:
pusha
call timer_tick_call
popa
iretd