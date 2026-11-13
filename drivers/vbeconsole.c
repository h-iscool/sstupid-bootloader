#include "vbeconsole.h"
#include "./vbe.h"
#include <stdint.h>
#include "./string.h"
#include"./fonts/psf2struct.h"
#include "./fonts/font1.h"

static int vbe_supported = 0;

static int height = 0;
static int width = 0;
static int cursor_x = 0;
static int cursor_y = 0;
static uint32_t bgColor = 0;
static uint32_t fgColor = 0;
static int maxCharsPerLine=0;
static int maxLines=0;
static float font_scaling = 1.0f;
static uint32_t *frameBuffer = 0;
static int pitch=0;
static uint32_t pxPerScanline = 0;
static uint32_t lineHeightPx = 0;
static int charHeight = 0;
static int charWidth = 0;

void scrollUp_lines(int lines){
    uint32_t scrollPx = lines*lineHeightPx;

    if (scrollPx>height){vbe_fill_screen(bgColor);return;}

    memcpy32_fast(frameBuffer, frameBuffer + (scrollPx * pxPerScanline), (height - scrollPx) * pxPerScanline);
    vbe_fill_rect(0, height-scrollPx, width, height, bgColor);

}



void draw_dbg_char_placement(){
    vbe_fill_rect(cursor_x*charWidth,cursor_y*charHeight,(cursor_x*charWidth)+charWidth,(cursor_y*charHeight)+charHeight,fgColor);
    cursor_x++;
    if (cursor_x>=maxCharsPerLine){
        cursor_x=0;
        cursor_y++;
    }
    if (cursor_y>=maxLines){
        cursor_y--;
        scrollUp_lines(1);
    }
}

void vbe_draw_char(char c, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg) {
    uint8_t uc = (uint8_t)c;

    //32 is the header junk and its 64 b / char
    const uint8_t *toPrint = &font1_psf[32 + (uc * 64)];

    for (int chary = 0; chary < charHeight; chary++) {
        int fonty = (chary * 32) / charHeight;

        // combine hi and lo bitews
        uint16_t row_bits = (toPrint[fonty * 2] << 8) | toPrint[(fonty * 2) + 1];

        uint32_t *fb_row = frameBuffer + ((y + chary) * pxPerScanline) + x;

        for (int charx = 0; charx < charWidth; charx++) {
            int src_x = (charx * 16) / charWidth;
            int isbitset = (row_bits >> (15 - src_x)) & 1;

            fb_row[charx] = isbitset ? fg : bg;
        }
    }
}

void vbe_print_string(const char *);

void vbe_print_char(char character){

    if (character=='\n'){
        cursor_x=0;
        cursor_y++;
        return;
    }
    if (character=='\r'){
        cursor_x=0;
        return;
    }
    if (character=='\t'){
        vbe_print_string("     ");
        return;
    }

    vbe_draw_char(character,cursor_x*charWidth,cursor_y*charHeight,fgColor,bgColor);
    cursor_x++;
    if (cursor_x>=maxCharsPerLine){
        cursor_x=0;
        cursor_y++;
    }
    if (cursor_y>=maxLines){
        cursor_y--;
        scrollUp_lines(1);
    }
}

void vbe_print_string(const char * string){
    for (int i = 0; string[i] != '\0'; i++){
        vbe_print_char(string[i]);
    }
    
}

void vbe_clearScreen(){
    vbe_fill_screen(bgColor);
    cursor_x=0;
    cursor_y=0;
}


int init_vbeConsole(int attempted_vbe_width,int attempted_vbe_height,uint32_t bgColorARGB,uint32_t fgColorARGB){
    vbe_supported = check_vbe_support();
    if (vbe_supported){
        vbe_scan_resolutions();
        vbe_supported = vbe_try_set_resolution(attempted_vbe_width,attempted_vbe_height);
        if (vbe_supported){ //after all setup do:

            charHeight=32*font_scaling;
            charWidth=16*font_scaling;

            width = attempted_vbe_width;
            height = attempted_vbe_height;
            bgColor = bgColorARGB;
            fgColor = fgColorARGB;
            cursor_x = 0;
            cursor_y = 0; 
            maxCharsPerLine = (width/charWidth); // 16x32font... maybe + scaling
            maxLines = (height/charHeight);
            frameBuffer = vbe_get_fb_ptr();
            pitch = vbe_get_pitch();
            pxPerScanline = pitch/4;
            lineHeightPx = charHeight;


            vbe_fill_screen(bgColor);
            
            return 1;

        }
        
    }
    return 0;
}

