#include <io/gpu.h>
#include <io/uart.h>
#include <utils/color.h>
#include <ui/view.h>
#include <io/gpio.h>
#include <ui/view_group.h>

void gpu_init(void) {
    // Aparantly, this sometimes does not work, so try in a loop
    while(framebuffer_init() != 0);

    // clear screen
    clear_screen();

    gpu_puts("\n");
    gpu_puts("\n");
    gpu_puts("\n");
    gpu_puts("\n");
    gpu_puts("                  GANAMOS AMIGOS");

    VIEW v = new_view(640, 20, 0, 0);
    v.bgColor = GREY;
    draw(&v);

    VIEW v2 = new_view(40, 40, 0, 0);
    v2.bgColor = RED;

    VIEW anim = new_view(40, 20, 80, 80);
    anim.bgColor = RED;

    VIEW_GROUP parent = new_view_group(200, 200, 200, 200);
    addView(&parent, &v2);
    addView(&parent, &anim);
    drawGroup(&parent);
}

void clear_screen() {
    for (uint32_t i = 0; i < fbinfo.width; i++) {
        for (uint32_t j = 0; j < fbinfo.height; j++) {
            write_pixel(i, j, &BLACK);
        }
    }
}

void write_pixel(uint32_t x, uint32_t y, color_24* pix) {
    //uint8_t * location = fbinfo.buf + y*fbinfo.pitch + x*BYTES_PER_PIXEL;
    if (x < fbinfo.width && y < fbinfo.height) {
        uint8_t* location = fbinfo.buf + y*fbinfo.pitch + x*BYTES_PER_PIXEL;
        memcpy(location, pix, BYTES_PER_PIXEL);
    }
}

void gpu_puts(char* string) {
    for (size_t i = 0; string[i] != '\0'; i++)
        gpu_putc((unsigned char)string[i]);
}

void gpu_putc(char c) {
    uint8_t w,h;
    uint8_t mask;
    const uint8_t* bmp = font(c);
    uint32_t i, num_rows = fbinfo.height/CHAR_HEIGHT;

    // shift everything up one row
    if (fbinfo.chars_y >= num_rows) {
        // Copy a whole character row into the one above it
        for (i = 0; i < num_rows-1; i++)
            memcpy(fbinfo.buf + fbinfo.pitch*i*CHAR_HEIGHT, fbinfo.buf + fbinfo.pitch*(i+1)*CHAR_HEIGHT, fbinfo.pitch * CHAR_HEIGHT);
        // zero out the last row
        bzero2(fbinfo.buf + fbinfo.pitch*i*CHAR_HEIGHT,fbinfo.pitch * CHAR_HEIGHT);
        fbinfo.chars_y--;
    }

    if (c == '\n') {
        fbinfo.chars_x = 0;
        fbinfo.chars_y++;
        return;
    }

    for(w = 0; w < CHAR_WIDTH; w++) {
        for(h = 0; h < CHAR_HEIGHT; h++) {
            mask = 1 << (w);
            if (bmp[h] & mask)
                write_pixel(fbinfo.chars_x*CHAR_WIDTH + w, fbinfo.chars_y*CHAR_HEIGHT + h, &WHITE);
            else
                write_pixel(fbinfo.chars_x*CHAR_WIDTH + w, fbinfo.chars_y*CHAR_HEIGHT + h, &BLACK);
        }
    }

    fbinfo.chars_x++;
    if (fbinfo.chars_x > fbinfo.chars_width) {
        fbinfo.chars_x = 0;
        fbinfo.chars_y++;
    }
}
