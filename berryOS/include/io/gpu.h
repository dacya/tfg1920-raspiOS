/**
 * Functions to render things into the screen
 * 
 * Tomás Golomb Durán - Feb. 14, 2020
 */

#ifndef GPU_H
#define GPU_H

#include <io/base.h>
#include <io/framebuffer.h>
#include <stdint.h>
#include <utils/fonts/chars_pixel.h>
#include <utils/stdlib.h>
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

/**
 * struct representing a pixel 
 */
typedef struct pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel_t;

#define BLACK ((pixel_t){0x00, 0x00, 0x00})
#define WHITE ((pixel_t){0xFF, 0xFF, 0xFF})
#define RED ((pixel_t){0xFF, 0x00, 0x00})
//const pixel_t RED = {0xFF, 0x00, 0x00};
//const pixel_t GREEN = {0x00, 0xFF, 0x00};
//const pixel_t BLUE = {0x00, 0x00, 0xFF};

/**
 * Initializes the gpu module by
 * initializing the framebuffer
 */
void gpu_init(void);

/**
 * Writes a pixel data into the
 * (x, y) point of the framebuffer which is related
 * to the screen coordinates. The (0, 0) origin is located
 * at the top-left corner.
 * 
 * GPU must be initialized @see gpu_init
 * @param x The x-axis value
 * @param y The y-axis value
 * @param pixel RGB color to set into the (x, y) pixel
 */
void write_pixel(uint32_t x, uint32_t y, pixel_t * pixel);

/**
 * Draws a char in the screen where the current pointer is located.
 * If c == '\n' then the pointer will jump to the next line.
 * Text will be scrolled automatically if the number of lines has surpassed 
 * the number of lines that fit in the screen.
 * 
 * GPU must be initialized @see gpu_init
 * @param pixel RGB color to set into the (x, y) pixel
 */
void gpu_putc(char c);

#endif
