;; // STAGE2 // ;;
;;  ;;

[bits 16]

; [org 0x7e00]; linker manages where file starts in memory
section .text.boot
global stage2_start ;;export stage2_start for linker
global call_load_testing ;; export for kernel.s to call if true (0x1)
;extern <linked call/jmp point>
;; call the extern later
;;when using linker, remove padding

extern kernel_init

;; https://en.wikipedia.org/wiki/BIOS_interrupt_call#Interrupt_table

stage2_start:
;;cleanup regs

xor eax, eax
mov ds,ax
mov es,ax

xor edi,edi ;;our data ptr

mov esi, successful_load_string
call print_until_null

;;basic console thing (I NEED A WAY OF INTERACTING WITH TS BEFORE I LOSE MY MIND):

mov esi, command_start_string
call print_until_null

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
    mov ebx, 0x7
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
	mov ebx, 0x7
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
        mov cx, 18 ;;how many chars (realizing null term is dumb if im defining ts)
        cld ;; clear direction flags (si and di inc forward)
        repe cmpsb ; repeat while equal, compare string bytes
        je enter_protected_mode


        ;;poweroff:
        mov si, charbuffer ;;string to check 1
        mov di, cmd_power_off ;;string to check 2
        mov cx, 8 ;;how many chars (realizing null term is dumb if im defining ts)
        cld ;; clear direction flags (si and di inc forward)
        repe cmpsb ; repeat while equal, compare string bytes
        je shutdown_command

        ;;vbe on:
        mov si, charbuffer ;;string to check 1
        mov di, cmd_vbe_on ;;string to check 2
        mov cx, 10 ;;how many chars (realizing null term is dumb if im defining ts)
        cld ;; clear direction flags (si and di inc forward)
        repe cmpsb ; repeat while equal, compare string bytes
        je vbe_enable_command

        ;;vbe off:
        mov si, charbuffer ;;string to check 1
        mov di, cmd_vbe_off ;;string to check 2
        mov cx, 11 ;;how many chars (realizing null term is dumb if im defining ts)
        cld ;; clear direction flags (si and di inc forward)
        repe cmpsb ; repeat while equal, compare string bytes
        je vbe_disable_command

        ;;enable testing call:
        mov si, charbuffer ;;string to check 1
        mov di, cmd_testing_call_on ;;string to check 2
        mov cx, 14 ;;how many chars (realizing null term is dumb if im defining ts)
        cld ;; clear direction flags (si and di inc forward)
        repe cmpsb ; repeat while equal, compare string bytes
        je debug_enable_command

        ;;disable testing call:
        mov si, charbuffer ;;string to check 1
        mov di, cmd_testing_call_off ;;string to check 2
        mov cx, 15 ;;how many chars (realizing null term is dumb if im defining ts)
        cld ;; clear direction flags (si and di inc forward)
        repe cmpsb ; repeat while equal, compare string bytes
        je debug_disable_command

        ;; help:

        mov si, charbuffer ;;string to check 1
        mov di, cmd_help ;;string to check 2
        mov cx, 4 ;;how many chars (realizing null term is dumb if im defining ts)
        cld ;; clear direction flags (si and di inc forward)
        repe cmpsb ; repeat while equal, compare string bytes
        je help_command

        ;;fallback

        mov si, text_invld_err_pt_1
        call print_until_null
        mov si, charbuffer
        call print_until_null
        mov si, text_invld_err_pt_2
        call print_until_null

        ;;not writing ts rn lmao

        clear_buffer:
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

        mov si, command_start_string
        call print_until_null


        jmp bootloaderConsoleLoop





;;attempted print fn rn:
print_until_null: ;; set si to str to print with null term
    mov ah, 0x0e
    mov bh, 0x00
    mov bl, 0x07
    print_loop:
    lodsb ;loads si into al and inc si but kinda safer (load string bytes)
    cmp al, 0x0
    je print_end
    int 0x10
    jmp print_loop
    print_end:
    ret


;;strings and data that i dindt put in a .data cause funny

;;strs:

successful_load_string: db 0x0d, 0x0a, "Stage 2 bootloader loaded.", 0x0d, 0x0a,0
command_start_string: db 0x0d, 0x0a, "> ",0
text_entering_32bit: db 0x0d, 0x0a, "Entering 32bit mode...",0x0d, 0x0a,0
text_apm_error: db 0x0d, 0x0a, "APM (advanced power management) ERROR",0x0d, 0x0a,"could not shutdown. manual shutdown at this time should not break anything",0
text_help_command: db 0x0d, 0x0a, "HELP:", 0x0d, 0x0a, "to get to this page, run help",0x0d, 0x0a, "commands:",0x0d, 0x0a, "ent protected_mode",0x0d, 0x0a, "-enters 32bit protected mode and attempts to start entry into a C program",0x0d, 0x0a, "shutdown",0x0d, 0x0a, "-power off the device",0x0d, 0x0a, "help",0x0d, 0x0a, "-display this page", 0x0d, 0x0a, "enable vbe", 0x0d, 0x0a, "-attempts to enable VBE mode when loading protected mode", 0x0d, 0x0a, "disable vbe", 0x0d, 0x0a, "-unsets the flag set by enable vbe", 0x0d, 0x0a,0
text_invld_err_pt_1: db 0x0d, 0x0a, "The command",0x0d, 0x0a,0
text_invld_err_pt_2: db 0x0d, 0x0a, "is not recognised, try help for a list of commands",0x0d, 0x0a,0
text_vbe_unsupported: db 0x0d, 0x0a, "VBE is unsupported", 0x0d, 0x0a,0

;;vars ish:
try_enable_vbe_on_load: dd 0x0


call_load_testing: dd 0x0

;;commands:

cmd_32bit: db "ent protected_mode", 0
cmd_power_off: db "shutdown", 0
cmd_vbe_on: db "enable vbe", 0
cmd_vbe_off: db "disable vbe", 0
cmd_testing_call_on: db "enable testing", 0
cmd_testing_call_off: db "disable testing", 0
cmd_help: db "help",0

charbuffer:
times 64 db 0 ;; 64 bytes reserved for char buffer - only allow 63 for null term strs.


help_command:
mov si, text_help_command
call print_until_null
jmp clear_buffer


vbe_enable_command:
    mov [try_enable_vbe_on_load], 0x1
    jmp clear_buffer

vbe_disable_command:
    mov [try_enable_vbe_on_load], 0x0
    jmp clear_buffer

debug_enable_command:
    mov [call_load_testing], 0x1
    jmp clear_buffer

debug_disable_command:
    mov [call_load_testing], 0x0
    jmp clear_buffer


;;shutdown command logic: https://stackoverflow.com/questions/21463908/x86-instructions-to-power-off-computer-in-real-mode https://forum.osdev.org/viewtopic.php?t=16990 https://github.com/cirosantilli/x86-bare-metal-examples/blob/7cff2a3fc93a636f8e253892af212a30c5a58697/apm_shutdown2.S
shutdown_command: ; https://wiki.osdev.org/APM
    mov ah, 0x53 ;;APM command code
    mov al, 0x00 ;;install check code
    xor bx, bx
    int 0x15
    jc APM_ERROR

    mov ah, 0x53 ;;apm command code
    mov al, 0x04 ; disconnect code
    xor bx,bx
    int 0x15
    ; jc .disconnect_err
    jc APM_ERROR

    mov ah, 0x53 ;;apm needs to stop writing over ah ;w;
    mov al, 0x01 ;;real mode interface
    xor bx,bx
    int 0x15
    jc APM_ERROR

    ;;enable pwr mngment:
    mov ah, 0x53
    mov al, 0x08 ; change pwr mgmt state
    mov bx, 0x0001 ;on all devices
    mov cx, 0x0001 ; to enabled
    int 0x15
    jc APM_ERROR

    ;;set power state:
    mov ah, 0x53
    mov al, 0x07 ; set power state
    mov bx, 0x0001 ; on all devices
    mov cx, 0x03 ; to off
    int 0x15
    jc APM_ERROR

    ; .disconnect_err:
    ; jmp APM_ERROR
    ; ; mov si, text_apm_error
    ; ; call print_until_null
    ; ; jmp clear_buffer


    APM_ERROR:
        mov si, text_apm_error
        call print_until_null
        jmp clear_buffer







;;VBE:

try_load_vbe:

    ;; get info:

    mov ax, 0x0
    mov es, ax
    mov di, 0x8000 ; 0x8000 will be a buffer for a minute to hold the 512 byte "VBE Controller Info Block"


    ;; idk buffer sig thing:
    mov dword [es:di], "VBE2"

    ;; get controler info:
    mov ax, 0x4f00
    int 0x10
    cmp ax, 0x004f ;; is vbe even supported????
    jne .vbe_unsupported

    ;;if vbe is supported, get the video mode list pointer
    ;; its 0x0e from start of controler Block

    mov si, [0x8000 + 0x0e] ;;offset mode list
    mov ax, [0x8000 + 0x10]
    mov fs, ax

    .vbe_mode_loop:
        mov cx, [fs:si] ;;get vbe mode id
        cmp cx, 0xffff
        je .vbe_loop_done

        add si, 2
        push si ;; #SAVE SI REG


        ;;query mode:
        mov ax, 0x0
        mov es, ax
        mov di, 0x8500 ;store mode info block
        mov ax, 0x4f01 ; grab mode info
        int 0x10
        pop si ; yay we got si back
        cmp ax, 0x004f ;;check if query succeeded
        jne .vbe_mode_loop ;; loop again if failed

        ;;checks:

        ;;Linear frame buffer:
        mov al, [0x8500]
        test al, 0x80 ; 0x80 is the lfb enabled bit so it being enabled means lfb is supported, test is lossless (al doesnt change)
        jz .vbe_mode_loop ;; if failed check, continue looping


        ;; is graphics mode
        test al, 0x10
        jz .vbe_mode_loop


        ;;check for 32bit corlor depth
        mov al, [0x8500 + 0x19]
        cmp al, 32
        jne .vbe_mode_loop

        ;; get res:
        mov dx, [0x8500 + 18] ; width
        mov bx, [0x8500 + 20] ;height


        ;;check res:

        ;;rn check for 1920x1080 with a fallback of 1280x720. maybe later try for 2 or 4 k but rn this is good enough lol
        cmp dx, 1920 ;;check for 1920 width
        jne .try_720p
        cmp bx, 1080
        je .found_vbe_mode

        .try_720p:
            cmp dx, 1280
            jne .vbe_mode_loop
            cmp bx, 720
            jne .vbe_mode_loop
            mov [backup_vbe_mode], cx
            jmp .vbe_mode_loop



    .vbe_loop_done:
        mov cx, [backup_vbe_mode]
        cmp cx, 0 ;; bad if no mode
        je .vbe_unsupported

        ;;fix crash, reset es to 0

        mov ax, 0x0
        mov es, ax
        mov di, 0x8500

        ;; reget details for best mode
        mov ax, 0x4f01
        mov di, 0x8500
        int 0x10

    .found_vbe_mode:
        mov dx, [0x8500 + 0x10] ;; get bytes per scan line for later
        mov [vbe_bpsl], dx
        mov dx, [0x8500 + 18] ;; reget width
        mov [vbe_width], dx ; store width to give to C kernel cause im not writing these shit graphics in asm
        mov dx, [0x8500 + 20] ;;re-get height
        mov [vbe_height], dx ;store height

        mov eax, [0x8500+0x28]
        mov [vbe_lfb_ptr],eax ;;save lfb ptr

        ;;set vid mode, 0x4000 enables lfb access
        or cx, 0x4000
        mov bx, cx
        mov ax, 0x4f02 ;set vbe mode
        int 0x10

        mov dword [vbe_success], 1
        ret
       

    .vbe_unsupported:
        mov dword [vbe_success], 0
        mov si, text_vbe_unsupported
        call print_until_null
        ret


    backup_vbe_mode: dw 0x0
    vbe_width: dw 0x0
    vbe_bpsl: dw 0x0
    vbe_height: dw 0x0
    vbe_success: dd 0x0
    vbe_lfb_ptr: dd 0x0 ;; pointer to video mem



;; 32bit mode:


enter_protected_mode: ;; https://wiki.osdev.org/Protected_Mode#Entering_Protected_Mode


mov si, text_entering_32bit
call print_until_null


cmp [try_enable_vbe_on_load], 0x1
jne skip_vbe

call try_load_vbe

skip_vbe:

;;disable cursor: https://wiki.osdev.org/Text_Mode_Cursor
mov dx, 0x3D4
mov al, 0xA
out dx, al
inc dx
mov al, 0x20
out dx, al


cli ;; DISABLES INTERRUPTS ;w;
lgdt [gdt_descriptor] ; loads gdt register with start of the global descriptor table
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



;; REMEMBER TO MOVE STACK PTR BEFORE ARGS LMAO
mov esp, 0x90000




;;arguments for kernel.s to push to C fn later

movzx eax, word [vbe_bpsl] ;;movzx and define as word for non 32bit tjomgs
push eax

xor eax, eax
;;arguments for kernel.s to push to C fn later

mov eax, [vbe_lfb_ptr]
push eax        ; uint32_t pointer to LFB  also this is arg 4 because rev order

xor eax, eax
movzx eax, word [vbe_height]
push eax

xor eax, eax
movzx eax, word [vbe_width]
push eax

xor eax, eax
mov  eax,  [vbe_success] ;;all are uint32_t
push eax




; hlt

jmp kernel_init

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
; times 1024 - ( $-$$ ) db 0
