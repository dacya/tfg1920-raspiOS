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

VIEW new_view(int width, int height, int x, int y);
void draw(VIEW* v);

#endif /* _VIEW_H */