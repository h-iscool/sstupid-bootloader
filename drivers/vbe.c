#include "vbe.h"
#include <stdint.h>

static unsigned int * lfb_ptr;
static unsigned int width;
static unsigned int height;
static unsigned int bytes_per_scan_line;
static unsigned int pixels_per_scan_line;

void vbe_init(unsigned int vbe_width, unsigned int vbe_height, unsigned int vbe_bytes_per_scan_line, unsigned int * lfb_pointer){

    lfb_ptr = lfb_pointer;
    width = vbe_width;
    height = vbe_height;
    bytes_per_scan_line = vbe_bytes_per_scan_line;
    pixels_per_scan_line = vbe_bytes_per_scan_line / 4;

}

//vbe does pixels like:
// [8bits blue] [8bits green] [8bits red] [8bits alpha]
//so 4bytes / pixel
// pixel / screen buffer starts at lfb

// also for now i will only have a basic draw rect fn because i dont want scaling to be an issue later

void vbe_draw_rect_px(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2){
    // COMMENT THIS FUNCTION OUT LATER ( OR REMOVE IT FROM THE HEADER FILE ) THIS IS ONLY FOR DEBUGGING CAUSE I WANT TO SEE VBE WORK

    //ok so um i lowk dont know how to do ts
    unsigned int rectcolor = 0xffaa55aa; // ARGB
    unsigned int bgcolor = 0xFFFFFFFF; // white bg cause i want to color all pixels cause funny
    
    for (int i = 0; i<height; i++){ //loop i = y

        for (int j = 0; j<pixels_per_scan_line; j++){ // j = x

            int offset = (i*pixels_per_scan_line + j );

            if ((i>y1 && i<y2) && (j>x1 && j<x2)){

                    lfb_ptr[offset] = rectcolor;

            }else{
                lfb_ptr[offset] = bgcolor;
            }

        }
        
    }

}


void hsv_to_rgb_baremetal(uint16_t h, uint8_t s, uint8_t v, uint8_t *r, uint8_t *g, uint8_t *b) {

    if (s == 0) {
        *r = v;
        *g = v;
        *b = v;
        return;
    }


    uint8_t sector = h / 60;
    

    uint8_t remainder = h % 60;

    uint8_t p = (v * (255 - s)) >> 8;
    uint8_t q = (v * (15300 - (s * remainder))) / 15300; 
    uint8_t t = (v * (15300 - (s * (60 - remainder)))) / 15300;

    switch (sector) {
        case 0:  *r = v; *g = t; *b = p; break;
        case 1:  *r = q; *g = v; *b = p; break;
        case 2:  *r = p; *g = v; *b = t; break;
        case 3:  *r = p; *g = q; *b = v; break;
        case 4:  *r = t; *g = p; *b = v; break;
        default: *r = v; *g = p; *b = q; break;
    }
}

int combine_r_g_b_to_bgra(uint8_t r,uint8_t g,uint8_t b){
    int blue_color = (int) b << 0;
    int green_color = (int) g << 8;
    int red_color = (int) r << 16;
    int alpha = 0xFF << 24 ;

    return (blue_color | green_color | red_color | alpha);
}

static uint16_t hue = 0;

static uint8_t red = 0;
static uint8_t green = 0;
static uint8_t blue = 0;



void advanceHue(){

    hsv_to_rgb_baremetal(hue,200,200,&red,&green,&blue);

    hue++;
    if (hue>=360){
        hue = 0;
    }
}

void vbe_draw_rgb_gaymer_bg_funni(){
    int hw = 0;
    for (int y = 0; y<height; y++){
        for (int x = 0; x<pixels_per_scan_line; x++){
            lfb_ptr[(y*pixels_per_scan_line+x)] = combine_r_g_b_to_bgra(red,green,blue);
            
        }

        if (hw>=5){advanceHue();hw=0;}hw++;


    }advanceHue();
}