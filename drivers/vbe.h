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

#endif