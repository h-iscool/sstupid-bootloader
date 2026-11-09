#include "vbe.h"

static unsigned int * lfb_ptr;
static unsigned int width;
static unsigned int height;
static unsigned int bytes_per_scan_line;

void vbe_init(unsigned int vbe_width, unsigned int vbe_height, unsigned int vbe_bytes_per_scan_line, unsigned int * lfb_pointer){

    lfb_ptr = lfb_pointer;
    width = vbe_width;
    height = vbe_height;
    bytes_per_scan_line = vbe_bytes_per_scan_line;

}

//vbe does pixels like:
// [8bits blue] [8bits green] [8bits red] [8bits alpha]
//so 4bytes / pixel
// pixel / screen buffer starts at lfb

// also for now i will only have a basic draw rect fn because i dont want scaling to be an issue later

