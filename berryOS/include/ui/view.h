#ifndef _VIEW_H
#define _VIEW_H

#include <utils/color.h>

typedef struct {
    int width;
    int height;
    int x;
    int y;
    color_24 bgColor;
} VIEW;

VIEW new_view(int width, int height, int x, int y);
void draw(VIEW v);

#endif /* _VIEW_H */