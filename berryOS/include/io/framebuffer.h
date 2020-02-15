/**
 * Library to initialize a framebuffer (shared memory) 
 * to communicate with the GPU.
 * 
 * Tomás Golomb Durán - Feb. 14, 2020
 */

#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#include <io/mailbox.h>
#include <mem/mem.h>
#include <io/gpu.h>

#define COLORDEPTH 24
#define BYTES_PER_PIXEL COLORDEPTH/8

typedef struct framebuffer_info {
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    void * buf;
    uint32_t buf_size;
    uint32_t chars_width;
    uint32_t chars_height;
    uint32_t chars_x;
    uint32_t chars_y;
} framebuffer_info_t;

framebuffer_info_t fbinfo;

/**
 * Initializes the framebuffer
 * 
 * @see fb_info
 */
int framebuffer_init(void);

#endif /* _FRAMEBUFFER_H */