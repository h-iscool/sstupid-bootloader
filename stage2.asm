[org 0x7e00]; start in memory according to stage 1
[bits 16]


;;cleanup ds reg

xor ax,ax
mov ds,ax

xor di,di ;;our data ptr



bootloaderConsoleLoop:



jmp bootloaderConsoleLoop

charbuffer:
times 64 db 0 ;; 64 bytes reserved for char buffer - only allow 63 for null term strs.

; padding for 1024 bytes (current s2 size)
times 1024 - ( $-$$ ) db 0