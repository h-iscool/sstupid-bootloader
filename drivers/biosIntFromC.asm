;;if this is still in fix stupid.asm later, um... move it at some point in time to a better file name

section .text.asmtoc ;; need to have this low in memory otherwise errors
[bits 32] ;; comp be happy

global real_mode_interrupt
global vbe_512_buffer

align 16
vbe_512_buffer:
times 512 db 0x0 ;;reserve 512 bytes (yes ik this is in the text area but this text area is right after stage 1 and stage 2 and there needs to be a 0% chance that this buffer is loaded over 1mb in ram (yes ikik the iso is too small for that to be possible but this is good futureproofing imo))

global vbe_256_mode_buffer
align 16
vbe_256_mode_buffer:
times 256 db 0x0

real_mode_interrupt:

push ebp
mov ebp, esp
pusha ;;pushes AX CX DX BX SP BP SI DI so we dont brake anythin

;; [ebp+0x8] = uint8_t interupt num
;; [esb+0xc] = ptr to struct with regs that are uint16_t

cli ;; make cpu shut up for a sec and lock tf in

sidt [idt32_ptr] ;;store the idt rn cause we no wanna braked it

mov dl, [ebp+0x8] 
mov [interupt_holder], dl ;;go to struct
mov esi, [ebp+0xc] 

mov ax, [esi+0x0] ;; dicect struct and move the pieces into the right "vars"
mov [ax_holder], ax
mov ax, [esi+0x2]
mov [bx_holder], ax 
mov ax, [esi+0x4]
mov [cx_holder], ax
mov ax, [esi+0x6]
mov [dx_holder], ax
mov ax, [esi+0x8]
mov [si_holder], ax
mov ax, [esi+0xa]
mov [di_holder], ax
mov ax, [esi+0xc]
mov [ds_holder], ax
mov ax, [esi+0xe]
mov [es_holder], ax


mov [esp_holder], esp

mov ax, 0x20
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

;;ok nows we kills halves of tehs 32bists

;; i love copy pasting gdt cause 2 vscode windows is 2 too many (im already at 1)

; gdt_code16:
; dw 0xffff, 0x0000, 0x9a00, 0x0000 ;;  offset: 0x18 
; gdt_data16:
; dw 0xffff, 0x0000, 0x9200, 0x0000 ;;  offset 0x20   
; gdt_end:

jmp 0x18:.into16bitsagain

[bits 16]
.into16bitsagain:
mov eax, cr0
and eax, ~1 ;; logical and by logical not 1 so we keep all data but the protection enable bit
mov cr0, eax
jmp 0x0:.realModeAgain

;; ^
;; |
;; |

;;"far jump" (ignore the ds part)


.realModeAgain:
xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7c00
lidt [cs:idt16_ptr] 

mov ax, [cs:ds_holder] ;;set regs to their stored values 
mov ds, ax 
mov ax, [cs:es_holder]
mov es, ax
mov ax, [cs:ax_holder]
mov bx, [cs:bx_holder]
mov cx, [cs:cx_holder]
mov dx, [cs:dx_holder]
mov di, [cs:di_holder]
mov si, [cs:si_holder]
; sti

opcode_int: ;;0xcd is the opcode for an interrupt https://shell-storm.org/x86doc/INT_n_INTO_INT_3.html //imagine writing asm like this... maybe later never in this project again tho i hope
db 0xcd 
interupt_holder: 
db 0x00

cli

mov [cs:ds_holder], ds
mov [cs:es_holder], es
mov [cs:bx_holder], bx
mov [cs:cx_holder], cx
mov [cs:dx_holder], dx
mov [cs:di_holder], di
mov [cs:si_holder], si

pushf
pop word [cs:flags_holder]
mov [cs:ax_holder], ax

mov eax, cr0
or eax, 1 ;;enable protected mode bit
mov cr0, eax

; gdt_start:
; ; dd 0x0, 0x0                        ;; offset: 0x00
; ; gdt_code:
; ; dw 0xffff, 0x0000, 0x9a00, 0x00cf ;;  offset: 0x08                                ;; 0xffff + f in 0x00cf = 0xfffff = 4gb max mem (max for 32bit) 0x0000 + 0x00 from 0x9a00 = start at 0x0 in ram
; ; gdt_data:
; ; dw 0xffff, 0x0000, 0x9200, 0x00cf ;;  offset: 0x10  


jmp dword 0x08:.Protected_Mode_2nd_Entry

[bits 32]
.Protected_Mode_2nd_Entry:
mov ax, 0x10
mov ds, ax
mov es, ax
mov ss, ax 


mov esp, [esp_holder]

lidt [idt32_ptr] ;; restore old idt


;; move reg holders to struct
mov esi, [ebp+0xc]
mov ax, [ax_holder]
mov [esi+0x0], ax
mov ax, [bx_holder]
mov [esi+0x2], ax
mov ax, [cx_holder]
mov [esi+0x4], ax
mov ax, [dx_holder]
mov [esi+0x6], ax
mov ax, [si_holder]
mov [esi+0x8], ax
mov ax, [di_holder]
mov [esi+0xa], ax
mov ax, [ds_holder]
mov [esi+0xc], ax
mov ax, [es_holder]
mov [esi+0xe], ax
mov ax, [flags_holder]
mov [esi+0x10], ax

popa ;;restore stuff that we stored before
pop ebp
;;here is where the legendary sti was
;; it caused a bootloop i couldnt find for a LONG time

sti ;;enable ints again
ret

align 4 ;; hax:
ax_holder: dw 0x0 ;; we dont believe in doing stuff the right way
bx_holder: dw 0x0 ;; so just shpve vars right next to code and hope they dont execute on accident
cx_holder: dw 0x0
dx_holder: dw 0x0
si_holder: dw 0x0
di_holder: dw 0x0
ds_holder: dw 0x0
es_holder: dw 0x0 
flags_holder: dw 0x0 
esp_holder: dd 0x0

idt16_ptr: ;; this keeps getting progressivlely worse
dw 0x03FF
dd 0x00000000

idt32_ptr:
dw 0
dd 0

;;even with using ai to try and help me bug fix this script took ~9hrs to make work.
;;ew
