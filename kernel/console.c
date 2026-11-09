#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../drivers/crypto.h"
#include "../drivers/string.h"
#include "syscall_wrappers.h"

static char consoleColorScheme = 0x0f;

#define PRINT_IDT 309003164 //adler32 hash of print-idt
#define PRINT_STACK 453837937 //adler32 hash of print-stack
#define HASH 68485541 //adler32 hash of "hash"
#define HELP 68878762 //adler32 hash of help

#define MAX_ARGS 6

void check_command(char command[]) {
	//vga_printString("Command entered: ",consoleColorScheme);
	size_t len = 0;
	while (command[len] != '\0' && command[len] != ' ') {
		len++;
	}
	//TODO:separate command[] into a command_array[], separated on spaces

	int hash = adler32((void *)command,len);

	char *numberString = malloc(sizeof(char)*20);
	vga_printString(itoa(hash,numberString,10),consoleColorScheme); //debug hash
	free(numberString);

	switch(hash) {
		case HELP: //help
			vga_printString(
			"Commands:\nprint-idt - print the address of the current interrupt descriptor table\nprint-stack - print the stack pointer\nhash - print hash of string/data\n"
			,consoleColorScheme);
			break;
		case PRINT_IDT: //print-idt
			vga_printString("IDT Pointer is at: ",consoleColorScheme);
			uintptr_t idt;
			__asm__ volatile ("sidt %0" : "=m"(idt));


			char *IDTaddressString = malloc(sizeof(char)*20);
			vga_printString(itoa(idt,IDTaddressString,16),consoleColorScheme);
			free(IDTaddressString);

			vga_printChar('\n',consoleColorScheme);
			break;
		case PRINT_STACK: //print-stack
			vga_printString("Stack Pointer is at: ",consoleColorScheme);

			uintptr_t sp;
			__asm__ volatile ("mov %%esp, %0" : "=r"(sp));	
			

			char *SPaddressString = malloc(sizeof(char)*20);
			vga_printString(itoa(sp,SPaddressString,16),consoleColorScheme);
			free(SPaddressString);

			vga_printChar('\n',consoleColorScheme);
			break;
		case HASH: //hash
			//TODO: ARGUMENTS
			vga_printString("Use the debug hashing in kernel/console.c\n",consoleColorScheme);
			break;
		default:
			vga_printString("No such command: ",consoleColorScheme);
			vga_printString(command,consoleColorScheme);
			vga_printChar('\n',consoleColorScheme);
	}

	return;
}

void vga_console() {

    consoleColorScheme = vga_pack_colors(VGA_COLOR_BLACK,VGA_COLOR_LIGHT_RED); 

    vga_clearScreen(consoleColorScheme);

    // vga_printString("print string function test\ndoes this work??", vga_pack_colors(VGA_COLOR_BLACK,VGA_COLOR_LIGHT_RED));

    while (true){

        char pressed = getPressedASCII();

        if (pressed != 0){
            vga_printChar(pressed, consoleColorScheme);
        }

    }
    

    
    // while(1) {
    //     // if ever gets to the end of the file, just hang
    //     __asm__("hlt");
    // }
}
