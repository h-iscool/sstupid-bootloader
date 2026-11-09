#ifndef VBE_H
#define VBE_H

void vbe_init(unsigned int width, unsigned int height, unsigned int bytes_per_scan_line, unsigned int * lfb_ptr);
void vbe_draw_rect_px(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void vbe_draw_rgb_gaymer_bg_funni();

#endif