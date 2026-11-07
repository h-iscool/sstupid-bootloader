[org 0x7e00]; start in memory according to stage 1
[bits 16]

global stage2_start ;;export stage2_start for linker
;extern <linked call/jmp point>
;; call the extern later
;;when using linker, remove padding


;; https://en.wikipedia.org/wiki/BIOS_interrupt_call#Interrupt_table

;;cleanup ds reg

xor ax,ax
mov ds,ax

xor di,di ;;our data ptr








;;basic console thing (I NEED A WAY OF INTERACTING WITH TS BEFORE I LOSE MY MIND):

mov ah, 0x0E
mov al, '>'
mov bh, 0x00
mov bl, 0x07
int 0x10
mov al, ' '
int 0x10
; the fancy "> " before a command

bootloaderConsoleLoop:

;; get keypress https://en.wikipedia.org/wiki/INT_16H 0x00

mov ah, 0x0
int 0x16

;; if enter then check buffer for what command it is and do what enter does (carriage return and line feed)
cmp al, 0x0d ; technically enter is the carriage return key ;w;
je console_enter_pressed

;;continues if not pressed

;;backspace check
cmp al, 0x08
je backspace_logic

;;if not bs or enter:

cmp di, 63
je bootloaderConsoleLoop ;; if buffer is full just ignore (63 cause null terminated)

;;continues if less than 63bytes
mov byte [charbuffer+di], al ;; add the currently pressed key (al) to the buffer
inc di

;;print char
mov ah, 0x0E
mov bh, 0x00
mov bl, 0x07
int 0x10

;;loop

jmp bootloaderConsoleLoop

;;backspace bs
backspace_logic:

;;no underflow
cmp di, 0x0
je bootloaderConsoleLoop

dec di
mov byte [charbuffer+di], 0
mov ah, 0x0E
mov al, 0x08
mov bh, 0x00
mov bl, 0x07
int 0x10
mov al, ' '
int 0x10
mov al, 0x08
int 0x10
jmp bootloaderConsoleLoop


;;command logic and enter stuff lol

console_enter_pressed:

mov bx, di ;;save di

;; 32bit:

mov si, charbuffer ;;string to check 1
mov di, cmd_32bit ;;string to check 2
mov cx, 5 ;;how many chars (realizing null term is dumb if im defining ts)
cld ;; clear direction flags (si and di inc forward)
repe cmpsb ; repeat while equal, compare string bytes
je enter_protected_mode

;; help/fallback:

;;not writing ts rn lmao


;;reset vars
mov di, bx ;;restore di


;; clear inline/incode/in.text/whatever "buffer"

xor bx, bx ;; temp loop idx
clear_buffer_loop:
mov byte [charbuffer+bx], 0 ;;write 0 to the current byte charbuffer[index(technically offset but whatever atp)]
inc bx
cmp bx, 64
jl clear_buffer_loop ;; if cmp is less than 64 jump

xor di, di ;;reset di (the actual pointer)


;; return to loop on a newline with fancy thing

mov ah, 0x0E
mov al, 0x0d
mov bh, 0x00
mov bl, 0x07
int 0x10
mov al, 0x0a
int 0x10
mov al, '>'
int 0x10
mov al, ' '
int 0x10


jmp bootloaderConsoleLoop


;;strings and data that i dindt put in a .data cause funny


;;commands:

cmd_32bit: db "32bit", 0



charbuffer:
times 64 db 0 ;; 64 bytes reserved for char buffer - only allow 63 for null term strs.













;; 32bit mode:




enter_protected_mode: ;; https://wiki.osdev.org/Protected_Mode#Entering_Protected_Mode
cli ;; DISABLES INTERRUPTS ;w;
lgdt [gdt_descriptor] ; loads gdt register with start of the global discriptor table
mov eax, cr0
or al, 1 ; turn on protection enable bit in control reg 0 (cr0)
mov cr0, eax

jmp 0x08:init_32bit

[bits 32] ;;32 bit code yayayayayyayay
init_32bit:
mov ax, 0x10
mov ds, ax
mov ss, ax
mov es, ax

mov esp, 0x90000

;;call into c file with linker (add tomorrow)

hlt ;;halt if ever returns


gdt_start:
dd 0x0, 0x0
gdt_code:
dw 0xffff, 0x0000, 0x9a00, 0x00cf ;; 0xffff + f in 0x00cf = 0xfffff = 4gb max mem (max for 32bit) 0x0000 + 0x00 from 0x9a00 = start at 0x0 in ram
gdt_data:
dw 0xffff, 0x0000, 0x9200, 0x00cf ;; yk just look it up: https://wiki.osdev.org/GDT_Tutorial https://wiki.osdev.org/Global_Descriptor_Table#Segment_Descriptor and some googling cause idk why some of this works tbh cause it seems like intel just smashed random flags together but whatever
gdt_end:

gdt_descriptor:
dw gdt_end - gdt_start - 1
dd gdt_start




; padding for 1024 bytes (current s2 size) REMOVE WHEN USING LINKER
times 1024 - ( $-$$ ) db 0