#ifndef STRING_H
#define STRING_H

#include <stddef.h>

void *memset(void * ptr, size_t val, size_t size);
void *memcpy(void * dest, const void * src, size_t size);
size_t strlen(const char * string);
int strcmp (const char * string_A, const char * string_B);
char * itoa (int value, char * strbuff, int base);


#endif
