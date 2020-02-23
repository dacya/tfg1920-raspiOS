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
#include <utils/color.h>

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

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
void write_pixel(uint32_t x, uint32_t y, color_24* pixel);

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
