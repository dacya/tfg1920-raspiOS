#include <ui/view_group.h>
#include <ui/view.h>
#include <utils/color.h>
#include <io/gpu.h>
#include <io/uart.h>
#include <utils/stdlib.h>
#include <mem/mem.h>

IMPLEMENT_LIST(VIEW_OR_GROUP);

void new_view_group(VIEW_GROUP* vg, int width, int height, int x, int y, void(*layout)(void*)) {
    INITIALIZE_LIST(vg->children, VIEW_OR_GROUP);

    vg->layout = layout;

    VIEW* v = &vg->view;
    new_view(v, width, height, x, y);
}

void addView(VIEW_GROUP* vg, VIEW* view) {
    VIEW_OR_GROUP* vog = (VIEW_OR_GROUP*)kmalloc(sizeof(VIEW_OR_GROUP));
    vog->child = view;
    vog->type = TYPE_VIEW;
    append_VIEW_OR_GROUP_list(&vg->children, vog);
}

void addViewGroup(VIEW_GROUP* vg, VIEW_GROUP* view) {
    VIEW_OR_GROUP* vog = (VIEW_OR_GROUP*)kmalloc(sizeof(VIEW_OR_GROUP));
    vog->child = view;
    vog->type = TYPE_VIEW_GROUP;
    append_VIEW_OR_GROUP_list(&vg->children, vog);
}

void removeView(VIEW_GROUP* vg, VIEW* view) {
    VIEW_OR_GROUP* vog = head_VIEW_OR_GROUP_list(&vg->children);
    int size = vg->children.size;
    int i = 0;
    while (i < size) {
        if (vog->child == view) {
            remove_VIEW_OR_GROUP_list(&vg->children, vog);
            kfree(vog);
            return;
        }
        i++;
    }
}

void removeViewGroup(VIEW_GROUP* vg, VIEW_GROUP* view) {
    VIEW_OR_GROUP* vog = head_VIEW_OR_GROUP_list(&vg->children);
    int size = vg->children.size;
    int i = 0;
    while (i < size) {
        if (vog->child == view) {
            remove_VIEW_OR_GROUP_list(&vg->children, vog);
            kfree(vog);
            return;
        }
        i++;
    }
}

void drawRelative(VIEW_GROUP*vg, VIEW* v) {
    v->x = vg->view.x + v->x;
    //v->width = MIN(vg->view.width - v->x, v->width);
    v->y = vg->view.y + v->y;
    //v->height = MIN(vg->view.height - v->y, v->height);
    draw(v);
}

void adjustGroupRelative(VIEW_GROUP* parent, VIEW_GROUP* child) {
    child->view.x = parent->view.x + child->view.x;
    child->view.y = parent->view.y + child->view.y;
}

void drawGroup(VIEW_GROUP* vg) {
    draw(&vg->view);
    vg->layout(vg);
    VIEW_OR_GROUP* node = start_iterate_VIEW_OR_GROUP_list(&vg->children);
    while (has_next_VIEW_OR_GROUP_list(&vg->children, node)) {
        node = next_VIEW_OR_GROUP_list(node);
        if (node->type == TYPE_VIEW_GROUP) {
            adjustGroupRelative(vg, node->child);
            drawGroup(node->child);
        } else if (node->type == TYPE_VIEW) {
            drawRelative(vg, node->child);
        }
    }
}

void* removeViewByIndex(VIEW_GROUP* vg, int index) {
    if (index < vg->children.size) {
        VIEW_OR_GROUP* node = start_iterate_VIEW_OR_GROUP_list(&vg->children);
        int i = 0;
        while (i < vg->children.size) {
            node = next_VIEW_OR_GROUP_list(node);
            i++;
        }
        remove_VIEW_OR_GROUP_immediate(&vg->children, node);
        void* child = node->child;
        kfree(node);
        return child;
    }

    return NULL;
}

VIEW_OR_GROUP* getViewByIndex(VIEW_GROUP* vg, int index) {
    if (index < vg->children.size) {
        VIEW_OR_GROUP* node = start_iterate_VIEW_OR_GROUP_list(&vg->children);
        int i = 0;
        while (i < vg->children.size) {
            node = next_VIEW_OR_GROUP_list(node);
            i++;
        }
        return node;
    }

    return NULL;
}