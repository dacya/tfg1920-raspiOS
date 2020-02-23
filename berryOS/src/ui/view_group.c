#include <ui/view_group.h>
#include <ui/view.h>
#include <utils/color.h>
#include <utils/list.h>

//IMPLEMENT_LIST(VIEW);

VIEW_GROUP new_view_group(int width, int height, int x, int y) {
    VIEW_GROUP vg;

    //vg.children = kmalloc();

    VIEW v = vg.view;
    v.width = width;
    v.height = height;
    v.x = x;
    v.y = y;
    v.bgColor = WHITE;

    return vg;
}

void draw_group(VIEW_GROUP vg) {
    draw(vg.view);
}