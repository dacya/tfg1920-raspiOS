#ifndef _VIEW_H
#define _VIEW_H

#include <utils/color.h>

typedef enum TEXT_ALIGN {
    CENTER,
    LEFT,
    RIGHT
} TEXT_ALIGN;

typedef struct VIEW {
    int width;
    int height;
    int x;
    int y;
    int fontSize;
    color_24 bgColor;
    color_24 textColor;

    char* text;
    TEXT_ALIGN textAlign;
    int textLines;
    int textOverflow;
    int padding;
} VIEW;

void new_view(VIEW* v, int width, int height, int x, int y);
void draw(VIEW* v);
void printView(VIEW *v);
void setText(VIEW* v, char* str);
void destroyView(VIEW* v);

#endif /* _VIEW_H */