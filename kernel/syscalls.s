[bits 32]

global syscall_malloc

;;heap is arbitrarily at 0x70000

heap_pointer:
dd 0x70000

;heap_

syscall_malloc:
mov eax, [ebp + 8]  ; size_t size
mov ecx, [heap_pointer]
lea eax, [ecx+eax]
mov [heap_pointer], eax
ret