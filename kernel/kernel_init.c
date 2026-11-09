#include <stddef.h>

#include "../drivers/vga.h"
#include "console.h"
#include "../drivers/vbe.h"


void kernel_init_stage_2 (unsigned int vbe_load_success, unsigned int vbe_screen_width, unsigned int vbe_screen_height, unsigned int * lfb_pointer, unsigned int bytes_per_scan_line){

    if (vbe_load_success == 0){
        // vga_clearScreen(0x0f);
        // vga_printString("LOADED VGA MODE",0x0f);

        vga_console();



    }else if (vbe_load_success == 1)
    {
        vbe_init(vbe_screen_width, vbe_screen_height, bytes_per_scan_line, lfb_pointer);
        vbe_debug();
    }
    

    while (1){
        __asm__("hlt");
    }

}