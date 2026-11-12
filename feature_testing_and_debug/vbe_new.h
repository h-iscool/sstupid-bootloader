#ifndef VBE_NEW_H
#define VBE_NEW_H
#include <stdint.h>


void check_vbe_support();
static int vbe_get_mode_info(uint16_t mode);
static int vbe_res_exists(uint16_t width,uint16_t height);
void vbe_scan_resolutions();
int vbe_try_set_resolution(uint16_t width, uint16_t height);

#endif