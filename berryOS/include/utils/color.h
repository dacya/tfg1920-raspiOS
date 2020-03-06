#ifndef _COLOR_H
#define _COLOR_H

#include <stdint.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_24;

#define BLACK ((color_24){0x00, 0x00, 0x00})
#define WHITE ((color_24){0xFF, 0xFF, 0xFF})
#define RED ((color_24){0xFF, 0x00, 0x00})
#define GREEN ((color_24){0x00, 0xFF, 0x00})
#define BLUE ((color_24){0x00, 0x00, 0xFF})
#define YELLOW ((color_24){0xFF, 0xFF, 0x00})
#define GREY ((color_24){0x30, 0x30, 0x30})

#endif /* _COLOR_H */