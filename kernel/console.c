#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../drivers/crypto.h"
#include "../drivers/string.h"

static char consoleColorScheme = 0x0f;
char *buffer = (char *)0x999900;

#define PRINT_IDT 309003164 //adler32 hash of print-idt
#define PRINT_STACK 453837937 //adler32 hash of print-stack
#define HASH 68485541 //adler32 hash of "hash"
#define HELP 68878762 //adler32 hash of help

void check_command(char command[]) {
	//vga_printString("Command entered: ",consoleColorScheme);

	int hash = adler32((void *)command,strlen(command));

	//vga_printString(itoa(hash,buffer,10),consoleColorScheme); //debug hash

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
			vga_printString(itoa(idt,buffer,16),consoleColorScheme);
			vga_printChar('\n',consoleColorScheme);
			break;
		case PRINT_STACK: //print-stack
			vga_printString("Stack Pointer is at: ",consoleColorScheme);

			uintptr_t sp;
			__asm__ volatile ("mov %%esp, %0" : "=r"(sp));	

			vga_printString(itoa(sp,buffer,16),consoleColorScheme);
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

	consoleColorScheme = vga_pack_colors(VGA_COLOR_BLACK,VGA_COLOR_BLUE); 
	vga_clearScreen(consoleColorScheme);

	int command_len = 0;
	char command[64];

	while (true){	//TODO:CHANGE TO INTERUPT CALLS

		char pressed = getPressedASCII();

		switch(pressed) {
		case 0:
			break;
		case KEY_NEWLINE:
		case KEY_CARRIAGE_RETURN:
			vga_printChar('\n',consoleColorScheme);
			command[command_len] = 0x0;
			command_len = 0; //reset the string
			check_command(command);
			break;
		case KEY_DELETE:
			command_len--;
			break;
		default:

			vga_printChar(pressed, consoleColorScheme);
			command[command_len] = pressed;
			command_len++;
		}

	}



	// while(1) {
	//     // if ever gets to the end of the file, just hang
	//     __asm__("hlt");
	// }
}
