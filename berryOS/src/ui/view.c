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
    v->fontSize = 1;
    v->padding = 1;
    v->textOverflow = 0;
    v->text = NULL;
    v->textAlign = LEFT;
    v->textLines = 1;
}

void write_pixel_inside(VIEW* v, int x, int y, color_24* color) {
    if (v->x <= x && v->y <= y)
        write_pixel(x, y, color);
}

void putChar(VIEW* v, char c, int posX, int offsetx, int offsety) {
    uint8_t w,h;
    uint8_t mask;
    const uint8_t* bmp = font(c);
    int charSize = CHAR_MIN_SIZE * v->fontSize;
    int charsPerLine = v->width / charSize;
    int x = posX % charsPerLine;
    int y;
    if (v->textOverflow)
        y = 0;
    else
        y = MIN(posX / charsPerLine, v->textLines);
    offsetx = offsetx + v->x;
    offsety = offsety + v->y;

    for(w = 0; w < CHAR_MIN_SIZE * v->fontSize; w++) {
        for(h = 0; h < CHAR_MIN_SIZE * v->fontSize; h++) {
            mask = 1 << (w / v->fontSize);
            if (bmp[h / v->fontSize] & mask)
                write_pixel_inside(v, offsetx + x * CHAR_MIN_SIZE * v->fontSize + w, offsety + y * CHAR_MIN_SIZE * v->fontSize + h, &v->textColor);
            else
                write_pixel_inside(v, offsetx + x * CHAR_MIN_SIZE * v->fontSize + w, offsety + y * CHAR_MIN_SIZE * v->fontSize + h, &v->bgColor);
        }
    }
}
#include <io/uart.h>
void draw(VIEW* v) {
    int width = v->x + v->width;
    int height = v->y + v->height;

    for (int i = v->x; i < width; i++) {
        for (int j = v->y; j < height; j++) {
            write_pixel(i, j, &v->bgColor);
        }
    }
    if (v->text != NULL) {
        int i = 0;
        int ssize = 0;
        char c;
        while ((c = v->text[ssize++]) != '\0');
        ssize--;
        int init = 0;
        if (v->textOverflow)
            init = MAX(ssize - v->width / (CHAR_MIN_SIZE * v->fontSize), 0);
        while ((c = v->text[i + init]) != '\0') {
            int offset;
            if (v->textAlign == LEFT)
                offset = v->padding;
            else if (v->textAlign == RIGHT)
                offset = v->width - MIN(CHAR_MIN_SIZE * v->fontSize * ssize, v->width) - v->padding;
            else
                offset = (v->width - MIN(CHAR_MIN_SIZE * v->fontSize * ssize, v->width)) / 2;
            putChar(v, c, i, offset, (v->height - CHAR_MIN_SIZE * v->textLines) / 2);
            i++;
        }
    }
}

void setText(VIEW* v, char* str) {
    if (v->text != NULL) {
        kfree(v->text);
    }
    int ssize = strlen(str) + 1; // add 1 extra for the \0
    v->text = kmalloc(sizeof(str) * ssize);
    memcpy(v->text, str, ssize);
}

void printView(VIEW* v) {
    uart_puts("X: ");
    uart_puts(itoa(v->x));
    uart_puts("y: ");
    uart_puts(itoa(v->y));
    uart_puts("W: ");
    uart_puts(itoa(v->width));
    uart_puts("H: ");
    uart_puts(itoa(v->height));
    uart_puts("\n");
}

void destroyView(VIEW* v) {
    if (v->text != NULL) {
        kfree(v->text);
    }
}