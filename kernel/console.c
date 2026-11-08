#include "../drivers/vga.h"
#include "../drivers/keyboard.h"

static char consoleColorScheme = 0x0f;

void vga_console() {

    consoleColorScheme = vga_pack_colors(VGA_COLOR_BLACK,VGA_COLOR_LIGHT_RED); 

    vga_clearScreen(consoleColorScheme);

    // vga_printString("print string function test\ndoes this work??", vga_pack_colors(VGA_COLOR_BLACK,VGA_COLOR_LIGHT_RED));

    while (true){

        char pressed = getPressedASCII();

        if (pressed != 0){
            vga_printChar(pressed, consoleColorScheme);
        }

    }
    

    
    // while(1) {
    //     // if ever gets to the end of the file, just hang
    //     __asm__("hlt");
    // }
}
