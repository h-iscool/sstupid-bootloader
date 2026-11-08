#include "vga_funcs.h"



void kernel_main() {

    vga_clearScreen(vga_pack_colors(VGA_COLOR_WHITE,VGA_COLOR_LIGHT_RED));

    vga_printString("print string function test\ndoes this work??", vga_pack_colors(VGA_COLOR_BLACK,VGA_COLOR_LIGHT_RED));


    
    while(1) {
        __asm__("hlt");
    }
}
