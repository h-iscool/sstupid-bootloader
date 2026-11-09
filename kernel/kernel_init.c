#include <stddef.h>

#include "../drivers/vga.h"
#include "console.h"


void kernel_init_stage_2 (unsigned int vbe_load_success, unsigned int vbe_screen_width, unsigned int vbe_screen_height, unsigned int * lfb_pointer){

    if (vbe_load_success == 0){
        // vga_clearScreen(0x0f);
        // vga_printString("LOADED VGA MODE",0x0f);

        vga_console();

    }

    while (1){
        __asm__("hlt");
    }

}