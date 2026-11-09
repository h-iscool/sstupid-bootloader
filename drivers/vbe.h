#ifndef VBE_H
#define VBE_H

void vbe_init(unsigned int width, unsigned int height, unsigned int bytes_per_scan_line, unsigned int * lfb_ptr);
void vbe_debug();

#endif