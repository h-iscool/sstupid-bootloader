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

void vbe_debug(){
    vbe_draw_rect_px(0,0,500,300);
}