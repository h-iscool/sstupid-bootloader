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
#define EXIT 71696827 //adler32 hash of exit
#define EXAMINE 196018920 //adler32 hash of examine

#define MAX_ARGS 6

void check_command(char command[]) {
	char *command_array[MAX_ARGS] = {0};
	size_t arguments = 0;

	size_t i = 0;
	while (command[i] != '\0') {
		while (command[i] == ' ') {
			command[i++] = '\0';
		}

		if (command[i] == '\0') {
			break;
		}

		if (arguments>=MAX_ARGS) break;
		command_array[arguments++] = &command[i];

		while (command[i] != '\0' && command[i] != ' ') {
			i++;
		}
	}

	if (arguments == 0) return;

	unsigned int hash = adler32((void *)command_array[0],strlen((char *)command_array[0]));

	switch(hash) {
		case HELP: //help
			printString(
			"Commands:\nprint-idt - print the address of the current interrupt descriptor table\nprint-stack - print the stack pointer\nhash - print hash of string/data\nexit - exit console back into the kernel\nexamine - read n bytes (base 10) from memory address (base 16). eg. examine ffff 80\n"
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
			//printString("Use the debug hashing in kernel/console.c\n");
			int hash = adler32((void *)command_array[1],strlen((char *)command_array[1]));
			char *hashString = malloc(sizeof(char)*20);
			printString(itoa(hash,hashString,10)); //debug hash
			free(hashString);
			printChar('\n');

			break;
		case EXAMINE: {
			void * address = (void *)atoi(command_array[1],strlen(command_array[1]),16);
			size_t size = (size_t)atoi(command_array[1],strlen(command_array[1]),10);

			char buffer[size+1];
			buffer[size] = (char)0x0; //last byte is length of buffer - 1

			memcpy((void *)buffer,(void *)address,size);
			
			printString(buffer);
			printChar('\n');
			//read hex data from an address
			break;
		}
		case EXIT: //exit
			//maybe do this later?
			break;
		default:
			printString("No such command: ");
			printString(command);
			printChar('\n');
	}

	return;
}

void console() {
	clearScreen();

	unsigned int command_len = 0;
	char command[64] = { 0 };

	while (true){	//TODO:CHANGE TO INTERUPT CALLS
		if (command_len>=64) {
			printChar('\n');
			command[63] = 0x0;
			command_len = 0; //reset the string
			check_command(command);
			continue;
		}

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
			if (command_len > 0){
				command[--command_len] = '\0';
			}
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
