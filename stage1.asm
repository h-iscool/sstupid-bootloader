;;// STAGE1 EXISTS SOLEY TO BOOT STAGE2 //;;

[org 0x7c00] ; the bootloader is the first 512 bytes of the drive but starts at 0x7c00 in memory for who knows why (i didnt look it up) so this just is being memory safe
[bits 16]

; LOAD STAGE 2:

mov bx, 0x7 
mov ah, 0xE
mov al, 'L'
int 0x10
mov al, 'o'
int 0x10
mov al, 'a'
int 0x10
mov al, 'd'
int 0x10
mov al, 'i'
int 0x10
mov al, 'n'
int 0x10
mov al, 'g'
int 0x10
mov al, ' '
int 0x10
mov al, 's'
int 0x10
mov al, 'e'
int 0x10
mov al, 'c'
int 0x10
mov al, 't'
int 0x10
mov al, 'o'
int 0x10
mov al, 'r'
int 0x10
mov al, 's'
int 0x10
mov al, '.'
int 0x10
mov al, '.'
int 0x10
mov al, '.'
int 0x10
mov al, 0xa
int 0x10
mov al, 0xd
int 0x10

; https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=02h:_Read_Sectors_From_Drive

;; NOTE:
;; up to 63 sectors / head ;; (1-63)
;; up to 256 heads / cylinder (0-255)
;; up to 1024 cylinders / drive (0-1023)

xor ax, ax
mov es, ax
mov bx, 0x7e00

mov ah, 0x02 ; read from drive
mov al, 16 ; load 2 sectors (1024 bytes)
mov ch, 0x0 ; cyl 0
mov cl, 2 ; start to read from sector 2 (INDEX STARTS AT 1 AND INCR. IN 512 SO SEC2 is RIGHT after 0xAA55)
mov dh, 0x0 ; head 0
;; dl is automatically the drive with the bootloader UNLESS changed
mov bx, 0x7e00 ; 0x200(512) + 0x7c00(start of bootloader in memory) = 0x7e00
int 0x13

jc driveloader

jmp 0x0000:0x7e00 ; resets the codeseg inc to 0 and jmps instruction pntr

driveloader: ;; ERR: 1
mov ah, 0x0E
mov al, 'E'
mov bx, 0x7
int 0x10
mov al, 'R'
int 0x10
mov al, 'R'
int 0x10
mov al, ':'
int 0x10
mov al, ' '
int 0x10
mov al, '1'
int 0x10
hlt



;;padding and bootloader sig:

times 510 - ( $-$$ ) db 0
dw 0xAA55
