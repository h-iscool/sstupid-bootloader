;;// STAGE1 EXISTS SOLEY TO BOOT STAGE2 //;;

[org 0x7c00] ; the bootloader is the first 512 bytes of the drive but starts at 0x7c00 in memory for who knows why (i didnt look it up) so this just is being memory safe
[bits 16]

; LOAD STAGE 2:


mov si, text_loading
call print_until_null

; https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=02h:_Read_Sectors_From_Drive

;; NOTE:
;; up to 63 sectors / head ;; (1-63)
;; up to 256 heads / cylinder (0-255)
;; up to 1024 cylinders / drive (0-1023)

xor ax, ax
mov es, ax
mov bx, 0x7e00

mov ah, 0x02 ; read from drive
mov al, 63 ; load 63 sectors (32256 bytes)
mov ch, 0x0 ; cyl 0
mov cl, 2 ; start to read from sector 2 (INDEX STARTS AT 1 AND INCR. IN 512 SO SEC2 is RIGHT after 0xAA55)
mov dh, 0x0 ; head 0
;; dl is automatically the drive with the bootloader UNLESS changed
mov bx, 0x7e00 ; 0x200(512) + 0x7c00(start of bootloader in memory) = 0x7e00
int 0x13

jc driveloader

jmp 0x0000:0x7e00 ; resets the codeseg inc to 0 and jmps instruction pntr


print_until_null: ;; set si to str to print with null term
    mov ah, 0x0e
    mov bx, 0x7 
    print_loop:
    lodsb ;loads si into al and inc si but kinda safer (load string bytes)
    cmp al, 0x0
    je print_end
    int 0x10
    jmp print_loop
    print_end:
    ret

;;strs:
text_err1: db "ERR: 1", 0
text_loading: db "Loading sectors...", 0xa, 0xd, 0


driveloader: ;; ERR: 1
mov si, text_err1
call print_until_null
hlt




;;padding and bootloader sig:

times 510 - ( $-$$ ) db 0
dw 0xAA55
