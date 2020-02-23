#include <ui/view.h>
#include <io/gpu.h>
#include <utils/color.h>
#include <io/uart.h>

VIEW new_view(int width, int height, int x, int y) {
    VIEW v;
    v.width = width;
    v.height = height;
    v.x = x;
    v.y = y;
    v.bgColor = WHITE;

    return v;
}

void draw(VIEW v) {
    int width = v.x + v.width;
    int height = v.y + v.height;

    for (int i = v.x; i < width; i++) {
        for (int j = v.y; j < height; j++) {
            write_pixel(i, j, &v.bgColor);
        }
    }
}