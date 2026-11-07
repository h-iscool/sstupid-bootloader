[org 0x7c00] ; the bootloader is the first 512 bytes of the drive but starts at 0x7c00 in memory for who knows why (i didnt look it up) so this just is being memory safe

mov ah, 0x0E
mov al, 'H'
mov bh, 0x00
mov bl, 0x07
int 0x10
mov ah, 0x0E
mov al, 'a'
mov bh, 0x00
mov bl, 0x07
int 0x10
mov ah, 0x0E
mov al, 'i'
mov bh, 0x00
mov bl, 0x07
int 0x10
mov ah, 0x0E
mov al, ' '
mov bh, 0x00
mov bl, 0x07
int 0x10
mov ah, 0x0E
mov al, ':'
mov bh, 0x00
mov bl, 0x07
int 0x10
mov ah, 0x0E
mov al, 'p'
mov bh, 0x00
mov bl, 0x07
int 0x10
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




; LOAD STAGE 2:

; https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=02h:_Read_Sectors_From_Drive

;; NOTE:
;; up to 63 sectors / head ;; (1-63)
;; up to 256 heads / cylinder (0-255)
;; up to 1024 cylinders / drive (0-1023)



mov ah, 0x02 ; read from drive
mov al, 0x02 ; load 2 sectors (1024 bytes)
mov ch, 0x0 ; cyl 0
mov cl, 0x02 ; start to read from sector 2 (INDEX STARTS AT 1 AND INCR. IN 512 SO SEC2 is RIGHT after 0xAA55)
mov dh, 0x0 ; head 0
;; dl is automatically the drive with the bootloader UNLESS changed
mov bx, 0x7e00 ; 0x200(512) + 0x7c00(start of bootloader in memory) = 0x7e00
int 0x13

jc driveloaderr

jmp 0x0000:0x7e00 ; resets the codeseg inc to 0 and jmps instruction pntr

driveloaderr: ;; ERR: 1
mov ah, 0x0E
mov al, 'E'
mov bh, 0x00
mov bl, 0x07
int 0x10
mov ah, 0x0E
mov al, 'R'
mov bh, 0x00
mov bl, 0x07
int 0x10
mov ah, 0x0E
mov al, 'R'
mov bh, 0x00
mov bl, 0x07
int 0x10
mov ah, 0x0E
mov al, ':'
mov bh, 0x00
mov bl, 0x07
int 0x10
mov ah, 0x0E
mov al, ' '
mov bh, 0x00
mov bl, 0x07
int 0x10
mov ah, 0x0E
mov al, '1'
mov bh, 0x00
mov bl, 0x07
int 0x10
hlt

;;padding and stuff:


times 510 - ( $-$$ ) db 0
dw 0xAA55