#ifndef VBE_H
#define VBE_H
#include <stdint.h>


int check_vbe_support();
static int vbe_get_mode_info(uint16_t mode);
static int vbe_res_exists(uint16_t width,uint16_t height);
void vbe_scan_resolutions();
int vbe_try_set_resolution(uint16_t width, uint16_t height);
void vbe_fill_screen(uint32_t color) ;
int vbe_get_width();
int vbe_get_height();
void vbe_fill_rect(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2,uint32_t color);
uint32_t * vbe_get_fb_ptr();
int vbe_get_pitch();

#endif