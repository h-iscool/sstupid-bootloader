#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stddef.h>

extern void *malloc(size_t size);
extern void free(void * ptr);

#endif
