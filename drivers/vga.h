#ifndef VGA_H
#define VGA_H

#define VGA_COLOR_BRIGHT_BIT 0x8
#define VGA_COLOR_BLINK_BIT 0x8
//bitwise or with a color to get corresponding light color

#define VGA_COLOR_BLACK 0x0
#define VGA_COLOR_BLUE 0x1
#define VGA_COLOR_GREEN 0x2
#define VGA_COLOR_CYAN 0x3
#define VGA_COLOR_RED 0x4
#define VGA_COLOR_MAGENTA 0x5
#define VGA_COLOR_BROWN 0x6
#define VGA_COLOR_LIGHT_GRAY 0x7

#define VGA_COLOR_DARK_GRAY 0x8
#define VGA_COLOR_LIGHT_BLUE 0x9
#define VGA_COLOR_LIGHT_GREEN 0xa
#define VGA_COLOR_LIGHT_CYAN 0xb
#define VGA_COLOR_LIGHT_RED 0xc
#define VGA_COLOR_LIGHT_MAGENTA 0xd
#define VGA_COLOR_YELLOW 0xe
#define VGA_COLOR_WHITE 0xf

void vga_clearScreen(char color);
void vga_printChar(char toPrint, char color);
void vga_printString(const char* string, char color);
char vga_pack_colors(char characterColor, char backgroundColor);


#endif
