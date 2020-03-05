#include <ui/view_group.h>
#include <ui/view.h>
#include <utils/color.h>
#include <io/gpu.h>
#include <io/uart.h>
#include <utils/stdlib.h>

IMPLEMENT_LIST(VIEW);

void new_view_group(VIEW_GROUP* vg, int width, int height, int x, int y, void(*layout)(void*)) {
    INITIALIZE_LIST2(vg->children, VIEW);
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
    int width = MIN(v->x + vg->view.x + v->width, vg->view.x + vg->view.width);
    int height = MIN(v->y + vg->view.y + v->height, vg->view.y + vg->view.height);
    for (int i = vg->view.x + v->x; i < width; i++) {
        for (int j = vg->view.y + v->y; j < height; j++) {
            write_pixel(i, j, &v->bgColor);
        }
    }
    uart_puts("\n");
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
