#include "vbeconsole.h"
#include "../drivers/vbe.h"
#include <stdint.h>

void init_vbeConsole(int attempted_vbe_width,int attempted_vbe_height,uint32_t colorARGB){
    if (check_vbe_support()){
        vbe_scan_resolutions();
        if (vbe_try_set_resolution(attempted_vbe_width,attempted_vbe_height)){ //after all setup do:

            vbe_fill_screen(colorARGB);

        }
        
    }
}
