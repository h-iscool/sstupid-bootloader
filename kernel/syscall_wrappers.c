//WRAPPERS FOR OUR SYSCALLS

#include <stddef.h>

extern void *syscall_malloc(size_t size);

void *malloc(size_t size) {
	//return (void *)0x90000000; //later change to an asm inline that raises an interrupt
	syscall_malloc(size);
}

void free(void * ptr){
	return;
}
