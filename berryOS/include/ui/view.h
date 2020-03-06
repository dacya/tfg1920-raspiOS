#ifndef _VIEW_H
#define _VIEW_H

#include <utils/color.h>
#include <utils/list.h>

typedef struct VIEW{
    int width;
    int height;
    int x;
    int y;
    color_24 bgColor;
    DEFINE_LINK(VIEW);
} VIEW;

void new_view(VIEW* v, int width, int height, int x, int y);
void draw(VIEW* v);

#endif /* _VIEW_H */