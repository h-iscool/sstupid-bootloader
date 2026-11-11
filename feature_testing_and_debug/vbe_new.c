#include "./vbe_new.h"
#include <stdint.h>

// typedef struct{

//     uint32_t width;
//     uint32_t height;

// } resolution_t;


// resolution_t vbe_getResolutions(){

    

//     return (resolution_t){0,0};

// }

//NEED real mode helper ;w;

typedef struct {
    uint16_t ax, bx, cx, dx;
    uint16_t si, di;
    uint16_t ds, es;
    uint16_t flags;
} bios_regs16_t;

extern void real_mode_interrupt(uint8_t int_num, bios_regs16_t *regs);

void bios_print_vga_char_int_test(char character){
    bios_regs16_t regs = {0};

    regs.ax = (0x0e << 8) | character; //bc ah is the high 8 bits of ax and al is the low bits so we bitshift the first 8 up then or
    regs.bx = 0x0007;

    real_mode_interrupt(0x10, &regs);
}