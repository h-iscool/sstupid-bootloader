#include "./vbe.h"
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

typedef struct {
    uint16_t mode_num;
    uint16_t width;
    uint16_t height;
    uint16_t pitch;
    uint8_t  bpp;
    uint32_t framebuffer;
} vbe_mode_entry_for_list_t;

extern void real_mode_interrupt(uint8_t int_num, bios_regs16_t *regs);


static uint32_t screen_framebuffer = 0;
static uint16_t screen_width = 0;
static uint16_t screen_height = 0;
static uint16_t screen_pitch = 0; //bytesperscanline is also called pitch and im getting tired of typing bytes per scan line
static uint8_t screen_bpp; //bits per px or color depth
uint32_t *fbptr = 0;


static int vbeAllowed = 0;



extern vbe_info_block_t vbe_512_buffer; //exported from asm
extern vbe_mode_info_t vbe_256_mode_buffer;

int check_vbe_support() {
    vbeAllowed = 0;
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
            
            vbeAllowed = 1;
            return 1;

        }
    }
    return 0;
}



//port of get mode inf
static int vbe_get_mode_info(uint16_t mode){
    if (!vbeAllowed){return 0;}
    bios_regs16_t regs = {0};
    uint32_t bufferaddr = (uint32_t)&vbe_256_mode_buffer;

    regs.ax = 0x4f01;
    regs.cx = mode;
    regs.es = (uint16_t)(bufferaddr>>4);
    regs.di = (uint16_t)(bufferaddr&0x0f);

    real_mode_interrupt(0x10, &regs);

    return(regs.ax == 0x004f);
}


static vbe_mode_entry_for_list_t availableModes[128]; // theres no solid max so we just are doing 128 ig 
static uint16_t availableModesIdx = 0;

static int vbe_res_exists(uint16_t width,uint16_t height){
    if (!vbeAllowed){return 0;}
    for (uint32_t i = 0; i < availableModesIdx; i++){
        if (availableModes[i].width == width && availableModes[i].height == height){
            return 1;
        }
    }
    return 0;
}

void vbe_scan_resolutions(){
    availableModesIdx = 0;
    if (!vbeAllowed){return;}
    uint32_t vmptr = vbe_512_buffer.video_mode_ptr;
    uint16_t * modes = (uint16_t*)(((vmptr>>16)<<4)+(vmptr&0xffff)); //i hate ptr translation

    for (int i=0; modes[i]!= 0xffff; i++){
        if (availableModesIdx>=128){
            break;
        }

        uint16_t mode = modes[i];
        if (!vbe_get_mode_info(mode)){continue;}

        if ((vbe_256_mode_buffer.attributes & 0x90) != 0x90){continue;} // keep if graphics mode and lfb support

        if (vbe_256_mode_buffer.memory_model != 0x4 && vbe_256_mode_buffer.memory_model != 0x6){continue;} // only keep [ normal rbg mdoels]

        if (vbe_256_mode_buffer.bpp != 32){continue;} //only keep wanted color depth (32)
        
        if (vbe_res_exists(vbe_256_mode_buffer.width,vbe_256_mode_buffer.height)){
            continue;
        }

        availableModes[availableModesIdx]=(vbe_mode_entry_for_list_t){
            .mode_num = mode,
            .width = vbe_256_mode_buffer.width,
            .height = vbe_256_mode_buffer.height,
            .pitch = vbe_256_mode_buffer.pitch,
            .bpp = vbe_256_mode_buffer.bpp,
            .framebuffer = vbe_256_mode_buffer.framebuffer
        };
        availableModesIdx++;
    
    }


}





int vbe_try_set_resolution(uint16_t width, uint16_t height){
    if (!vbeAllowed){return 0;}
    for (uint32_t i = 0; i<availableModesIdx; i++){
        if (availableModes[i].width == width && availableModes[i].height == height){

            bios_regs16_t regs = {0};
            regs.ax = 0x4f02; //set
            regs.bx = availableModes[i].mode_num | 0x4000; // bit 0x4000 = lfb enabled

            real_mode_interrupt(0x10,&regs);

            if (regs.ax == 0x004f){
                screen_framebuffer = availableModes[i].framebuffer;
                screen_width = availableModes[i].width;
                screen_height = availableModes[i].height;
                screen_pitch = availableModes[i].pitch;
                screen_bpp = availableModes[i].bpp;
                fbptr = (uint32_t *)(uintptr_t)screen_framebuffer;
                return 1;
            }
            return 0;

        }
    }
    return 0; //not in list
}

void vbe_fill_screen(uint32_t color) {
    if (!screen_framebuffer || screen_bpp != 32) {
        return;
    }

    for (int y = 0; y<screen_height;y++){
        for (int x = 0; x<(screen_pitch/4);x++){
            fbptr[(x+(y*(screen_pitch/4)))] = color;
        }
    }
}