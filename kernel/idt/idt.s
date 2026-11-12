[bits 32]

global load_idt

load_idt:
mov eax, [esp+4] ;; esp+4 is the arg given from c
lidt [eax] ;;load idt from arg given
ret