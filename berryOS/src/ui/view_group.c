#include <ui/view_group.h>
#include <ui/view.h>
#include <utils/color.h>
#include <io/gpu.h>
#include <io/uart.h>
#include <utils/stdlib.h>

IMPLEMENT_LIST(VIEW);

void new_view_group(VIEW_GROUP* vg, int width, int height, int x, int y, void(*layout)(void*)) {
    INITIALIZE_LIST(vg->children, VIEW);
    vg->layout = layout;

    VIEW* v = &vg->view;
    v->width = width;
    v->height = height;
    v->x = x;
    v->y = y;
    v->bgColor = WHITE;
}

void addView(VIEW_GROUP* vg, VIEW* view) {
    append_VIEW_list(&vg->children, view);
}

void removeView(VIEW_GROUP* vg, VIEW* view) {
    remove_VIEW_list(&vg->children, view);
}

void drawRelative(VIEW_GROUP*vg, VIEW* v) {
    v->x = vg->view.x + v->x;
    v->y = vg->view.y + v->y;
    draw(v);
}

void drawGroup(VIEW_GROUP* vg) {
    draw(&vg->view);
    VIEW* node = head_VIEW_list(&vg->children);
    vg->layout(vg);
    while (node != &vg->children.ghost) {
        drawRelative(vg, node);
        node = node->nextVIEW;
    }
}