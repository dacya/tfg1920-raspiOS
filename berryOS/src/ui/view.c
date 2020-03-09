#include <ui/view.h>
#include <io/gpu.h>
#include <utils/color.h>
#include <io/uart.h>

void new_view(VIEW* v, int width, int height, int x, int y) {
    v->width = width;
    v->height = height;
    v->x = x;
    v->y = y;
    v->bgColor = BLACK;
    v->textColor = WHITE;
    v->fontSize = 2;
    v->text = NULL;
}

void write_pixel_inside(VIEW* v, int x, int y, color_24* color) {
    if (v->x <= x && v->y <= y)
        write_pixel(x, y, color);
}

void putChar(VIEW* v, char c, int posX) {
    uint8_t w,h;
    uint8_t mask;
    const uint8_t* bmp = font(c);
    int charSize = CHAR_MIN_SIZE * v->fontSize;
    int charsPerLine = v->width / charSize;
    int x = posX % charsPerLine;
    int y = posX / charsPerLine;
    for(w = 0; w < CHAR_MIN_SIZE * v->fontSize; w++) {
        for(h = 0; h < CHAR_MIN_SIZE * v->fontSize; h++) {
            mask = 1 << (w / v->fontSize);
            if (bmp[h / v->fontSize] & mask)
                write_pixel_inside(v, v->x + x * CHAR_MIN_SIZE * v->fontSize + w, v->y + y * CHAR_MIN_SIZE * v->fontSize + h, &v->textColor);
            else
                write_pixel_inside(v, v->x + x * CHAR_MIN_SIZE * v->fontSize + w, v->y + y * CHAR_MIN_SIZE * v->fontSize + h, &v->bgColor);
        }
    }
}

void draw(VIEW* v) {
    int width = v->x + v->width;
    int height = v->y + v->height;

    for (int i = v->x; i < width; i++) {
        for (int j = v->y; j < height; j++) {
            write_pixel(i, j, &v->bgColor);
        }
    }
    v->text = "HOLA";
    if (v->text != NULL) {
        int i = 0;
        char c;
        uart_puts("HAS TEXT\n");
        while ((c = v->text[i]) != '\0') {
            putChar(v, c, i);
            i++;
            uart_puts("1 CHAR\n");
        }
    }
}

void setText(VIEW* v, char* text) {
    v->text = text;
    draw(v);
}