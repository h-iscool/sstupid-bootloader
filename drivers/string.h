#ifndef STRING_H
#define STRING_H

#include <stddef.h>

#include <limits.h>

#if UINT_MAX != 0xFFFFFFFFU
#error "UNSIGNED INT IS NOT 32 BITS"
#endif

void *memset(void * ptr, size_t val, size_t size);
void *memcpy(void * dest, const void * src, size_t size);
size_t strlen(const char * string);
int strcmp (const char * string_A, const char * string_B);
char * itoa (int value, char * strbuff, size_t base);
int atoi (char *string, size_t len,size_t base);
void *memcpy32(void * dest, const void * src, size_t size);
void *memcpy32_fast(void *dest, const void *src, size_t count);


#endif
