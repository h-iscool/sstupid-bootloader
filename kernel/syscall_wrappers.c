//WRAPPERS FOR OUR SYSCALLS

#include <stddef.h>

void *malloc(size_t size) {
	return (void *)0x90000000; //later change to an asm inline that raises an interrupt
}

void free(void * ptr){
	return;
}
