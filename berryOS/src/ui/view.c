#include <ui/view.h>
#include <io/gpu.h>
#include <utils/color.h>
#include <io/uart.h>

void new_view(VIEW* v, int width, int height, int x, int y) {
    v->width = width;
    v->height = height;
    v->x = x;
    v->y = y;
    v->bgColor = WHITE;
}

void draw(VIEW* v) {
    int width = v->x + v->width;
    int height = v->y + v->height;

    for (int i = v->x; i < width; i++) {
        for (int j = v->y; j < height; j++) {
            write_pixel(i, j, &v->bgColor);
        }
    }
}
