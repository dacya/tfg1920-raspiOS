#include <ui/view_group.h>
#include <ui/view.h>
#include <utils/color.h>
#include <utils/list.h>
#include <io/gpu.h>
#include <io/uart.h>

IMPLEMENT_LIST(VIEW);

VIEW_GROUP new_view_group(int width, int height, int x, int y) {
    VIEW_GROUP vg;
    INITIALIZE_LIST2(vg.children, VIEW);
    uart_hex_puts(vg.children.ghost.nextVIEW);

    VIEW* v = &vg.view;
    v->width = width;
    v->height = height;
    v->x = x;
    v->y = y;
    v->bgColor = WHITE;

    return vg;
}

void addView(VIEW_GROUP* vg, VIEW* view) {
    append_VIEW_list(&vg->children, view);
}

void removeView(VIEW_GROUP* vg, VIEW* view) {
    remove_VIEW_list(&vg->children, view);
}

void drawRelative(VIEW_GROUP*vg, VIEW* v) {
    int width = vg->view.width > v->width ? v->width : vg->view.width;
    int height = vg->view.height > v->height ? v->height : vg->view.height;
    uart_puts(itoa(width));
    uart_puts(", h:");
    uart_puts(itoa(height));
    uart_puts(", x:");
    uart_puts(itoa(vg->view.x + v->x));
    uart_puts(", y:");
    uart_puts(itoa(vg->view.y + v->y));
    uart_puts("]\n");
    for (int i = vg->view.x + v->x; i < width; i++) {
        for (int j = vg->view.y + v->y; j < height; j++) {
            write_pixel(i, j, &v->bgColor);
        }
    }
    uart_puts("\n");
}

void drawGroup(VIEW_GROUP* vg) {
    //draw(&vg->view);
    VIEW* node = head_VIEW_list(&vg->children);
    uart_hex_puts(node);
    while (node != vg->children.pghost) {
        uart_puts("drawing child [w:");
        drawRelative(vg, node);
        node = node->prevVIEW;
    }
}
