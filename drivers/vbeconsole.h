#ifndef VBECONSOLE_H
#define VBECONSOLE_H

#include <stdint.h>

int init_vbeConsole(int attempted_vbe_width,int attempted_vbe_height,uint32_t bgColorARGB,uint32_t fgColorARGB);
void vbe_clearScreen();
void vbe_print_string(const char * string);
void vbe_print_char(char character);

#endif