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

typedef struct __attribute__((packed)){
    uint16_t ax, bx, cx, dx;
    uint16_t si, di;
    uint16_t ds, es;
    uint16_t flags;
} bios_regs16_t;


typedef struct __attribute__((packed)) { //packed attribute = no padding from comp
    char signature[4]; //whenm checkling for vbe this needs to be vbe2 as inoput and vesa as output (allcaps)
    uint16_t version; // self explanitory, when output if vbe then rthis is the version
    uint32_t oem_string_ptr; // seg:off to oem str
    uint32_t capabilities;
    uint32_t video_mode_ptr; // SUPPORTED VIDEO MODES PTR this one is good
    uint16_t total_memory; //num of 64kb areas
    uint8_t reserved[492]; //struct padding
} vbe_info_block_t;

typedef struct __attribute__((packed)) {
    uint16_t attributes;
    uint8_t window_a;
    uint8_t window_b;
    uint16_t granularity;
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;
    uint16_t pitch; // aka bytes per scan line
    uint16_t width;
    uint16_t height;
    uint8_t w_char;
    uint8_t y_char;
    uint8_t planes;
    uint8_t bpp;
    uint8_t banks;
    uint8_t memory_model;
    uint8_t bank_size;
    uint8_t image_pages;
    uint8_t reserved0;
    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;
    uint32_t framebuffer; // 0x28 LFB addr (phys)
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size;
    uint8_t reserved1[206];
} vbe_mode_info_t;

extern void real_mode_interrupt(uint8_t int_num, bios_regs16_t *regs);

void bios_print_vga_char_int_test(char character){
    bios_regs16_t regs = {0};

    regs.ax = (0x0e << 8) | character; //bc ah is the high 8 bits of ax and al is the low bits so we bitshift the first 8 up then or
    regs.bx = 0x0007;

    real_mode_interrupt(0x10, &regs);
}



extern vbe_info_block_t vbe_512_buffer; //exported from asm
extern vbe_mode_info_t vbe_256_mode_buffer;

void check_vbe_support() {
    bios_regs16_t regs = {0};

    // get the phys addr of the bujffer
    uint32_t buffer_addr = (uint32_t)&vbe_512_buffer;

    //set sig to VBE2 to request vbe2 info
    vbe_512_buffer.signature[0] = 'V';
    vbe_512_buffer.signature[1] = 'B';
    vbe_512_buffer.signature[2] = 'E';
    vbe_512_buffer.signature[3] = '2';

    //convert addr to real mode offset based thing

    //heres maths that make that woprk:
    //phys addr = segment*16 + offset
    //split 32bit addr into offset (di) and segment (es)
    // es is the addr >> 4 and cast to uint16_t to get everything but the last 4 bits
    //di is the addr bitwise and 0x0F because we only want to keep the last 4 bits then cast to uint16_t
    regs.es = (uint16_t)(buffer_addr >> 4);
    regs.di = (uint16_t)(buffer_addr & 0x0F);

    //set ax to the vbe info check
    regs.ax = 0x4F00;

    real_mode_interrupt(0x10, &regs); //video int

    if (regs.ax == 0x004F) { //VBE is enabled if the signiture returned is VESA
        if (vbe_512_buffer.signature[0] == 'V' && vbe_512_buffer.signature[1] == 'E' && vbe_512_buffer.signature[2] == 'S' && vbe_512_buffer.signature[3] == 'A') {
            
            bios_print_vga_char_int_test('1');

        }
    }
}