#ifndef PSF2STRUCT_H
#define PSF2STRUCT_H

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
} PSF2_Header;

#endif