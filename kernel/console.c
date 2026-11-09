#include "console.h"

#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../drivers/crypto.h"
#include "../drivers/string.h"
#include "syscall_wrappers.h"

#define PRINT_IDT 309003164 //adler32 hash of print-idt
#define PRINT_STACK 453837937 //adler32 hash of print-stack
#define HASH 68485541 //adler32 hash of "hash"
#define HELP 68878762 //adler32 hash of help
#define EXIT 0 //adler32 hash of exit

#define MAX_ARGS 6

extern char vga_colorScheme;

void check_command(char command[]) {
	//printString("Command entered: ");
	size_t len = 0;
	while (command[len] != '\0' && command[len] != ' ') {
		len++;
	}
	//TODO:separate command[] into a command_array[], separated on spaces

	int hash = adler32((void *)command,len);

	char *numberString = malloc(sizeof(char)*20);
	printString(itoa(hash,numberString,10)); //debug hash
	free(numberString);

	switch(hash) {
		case HELP: //help
			printString(
			"Commands:\nprint-idt - print the address of the current interrupt descriptor table\nprint-stack - print the stack pointer\nhash - print hash of string/data\nexit - exit console back into the kernel\n"
			);
			break;
		case PRINT_IDT: //print-idt
			printString("IDT Pointer is at: ");
			uintptr_t idt;
			__asm__ volatile ("sidt %0" : "=m"(idt));


			char *IDTaddressString = malloc(sizeof(char)*20);
			printString(itoa(idt,IDTaddressString,16));
			free(IDTaddressString);

			printChar('\n');
			break;
		case PRINT_STACK: //print-stack
			printString("Stack Pointer is at: ");

			uintptr_t sp;
			__asm__ volatile ("mov %%esp, %0" : "=r"(sp));	
			

			char *SPaddressString = malloc(sizeof(char)*20);
			printString(itoa(sp,SPaddressString,16));
			free(SPaddressString);

			printChar('\n');
			break;
		case HASH: //hash
			//TODO: ARGUMENTS
			printString("Use the debug hashing in kernel/console.c\n");
			break;
		case EXIT: //exit
			//maybe do this later?
		default:
			printString("No such command: ");
			printString(command);
			printChar('\n');
	}

	return;
}

void console() {
	initScreen(DRIVER_VGA);
	clearScreen();

	int command_len = 0;
	char command[64];

	while (true){	//TODO:CHANGE TO INTERUPT CALLS

		char pressed = getPressedASCII();

		switch(pressed) {
		case 0:
			break;
		case KEY_NEWLINE:
		case KEY_CARRIAGE_RETURN:
			printChar('\n');
			command[command_len] = 0x0;
			command_len = 0; //reset the string
			check_command(command);
			break;
		case KEY_DELETE:
			command_len--;
			break;
		default:

			printChar(pressed);
			command[command_len] = pressed;
			command_len++;
		}

	}



	// while(1) {
	//     // if ever gets to the end of the file, just hang
	//     __asm__("hlt");
	// }
}
