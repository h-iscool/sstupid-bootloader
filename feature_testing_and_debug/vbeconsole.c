#include "vbeconsole.h"
#include "../drivers/vbe.h"
#include <stdint.h>

static int vbe_supported = 0;

static int height = 0;
static int width = 0;
static int cursor_x = 0;
static int cursor_y = 0;
static uint32_t bgColor = 0;
static uint32_t fgColor = 0;
static int maxCharsPerLine=0;
static int font_scaling = 1;


void init_vbeConsole(int attempted_vbe_width,int attempted_vbe_height,uint32_t bgColorARGB,uint32_t fgColorARGB){
    vbe_supported = check_vbe_support();
    if (vbe_supported){
        vbe_scan_resolutions();
        vbe_supported = vbe_try_set_resolution(attempted_vbe_width,attempted_vbe_height);
        if (vbe_supported){ //after all setup do:

            width = attempted_vbe_width;
            height = attempted_vbe_height;
            bgColor = bgColorARGB;
            fgColor = fgColorARGB;
            cursor_x = 0;
            cursor_y = 0; 
            maxCharsPerLine = (width/16)/font_scaling; // 16x32font... maybe + scaling
            


            vbe_fill_screen(bgColor);

        }
        
    }
}
