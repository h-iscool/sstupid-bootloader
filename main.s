mov sp, 0x55555500

mov ah, 0xE
movzx bl, 0x7

mov al, 'H'
int 0x10
mov al, 'a'
int 0x10
mov al, 'i'
int 0x10
mov al, ' '
int 0x10
mov al, ':'
int 0x10
mov al, 'P'
int 0x10
mov al, 0x0a
int 0x10
mov al, 0x0d

mov ax, [ip+string]
call write_string

point:
mov ah, 0x00
int 0x16 #get key press

cmp al, 0x08
je backspace

cmp al, 0x0D
je enter

mov ah, 0x0E
mov bh, 0x00
mov bl, 0x07
int 0x10
jmp point

write_string:
	#ax -> pointer to null terminated string
	mov di, ax
	xor cx, cx
	movzx bl, 0x7
	.write_string_loop:
		test [di+cx], 0x0
		jz .write_string_skip
		ret
		.write_string_skip:
		movzx al, [di+cx]
		inc cx
	jmp .write_string_loop

backspace:
mov ah, 0x0E
mov bh, 0x00
mov bl, 0x07
int 0x10
mov ah, 0x0E
mov al, 0x20
mov bh, 0x00
mov bl, 0x07
int 0x10
mov ah, 0x0E
mov al, 0x08
mov bh, 0x00
mov bl, 0x07
int 0x10
jmp point

enter:
mov ah, 0x0E
mov al, 0x0A
mov bh, 0x00
mov bl, 0x07
int 0x10
mov ah, 0x0E
mov al, 0x0D
mov bh, 0x00
mov bl, 0x07
int 0x10
jmp point

string:
.asciiz "Hello world"

times 510 - ( $-$$ ) db 0 #pad up until the special dword
dw 0xAA55
