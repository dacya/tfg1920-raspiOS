#include <ui/layouts.h>
#include <utils/stdlib.h>
#include <io/uart.h>

void horizontal_linear_layout(void* self) {
    VIEW_GROUP* vg = (VIEW_GROUP*)self;
    int ccount = vg->children.size;
    if (ccount == 0) {
        return;
    }

    int x = 0;
    VIEW_OR_GROUP* node = head_VIEW_OR_GROUP_list(&vg->children);
    for (int i = 0; i < ccount; i++) {
        if (node->type == TYPE_VIEW) {
            ((VIEW*)node->child)->x = x;
            ((VIEW*)node->child)->y = 0;
            x += ((VIEW*)node->child)->width;
        } else if (node->type == TYPE_VIEW_GROUP) {
            ((VIEW_GROUP*)node->child)->view.x = x;
            ((VIEW_GROUP*)node->child)->view.y = 0;
            x += ((VIEW_GROUP*)node->child)->view.width;
        }
        node = next_VIEW_OR_GROUP_list(node);
    }
}

void vertical_linear_layout(void* self) {
    VIEW_GROUP* vg = (VIEW_GROUP*)self;
    int ccount = vg->children.size;
    if (ccount == 0) {
        return;
    }

    int y = 0;
    VIEW_OR_GROUP* node = head_VIEW_OR_GROUP_list(&vg->children);
    for (int i = 0; i < ccount; i++) {
        if (node->type == TYPE_VIEW) {
            ((VIEW*)node->child)->x = 0;
            ((VIEW*)node->child)->y = y;
            y += ((VIEW*)node->child)->height;
        } else if (node->type == TYPE_VIEW_GROUP) {
            ((VIEW_GROUP*)node->child)->view.x = 0;
            ((VIEW_GROUP*)node->child)->view.y = y;
            y += ((VIEW_GROUP*)node->child)->view.height;
        }
        node = next_VIEW_OR_GROUP_list(node);
    }
}

void horizontal_fitted_linear_layout(void* self) {
    VIEW_GROUP* vg = (VIEW_GROUP*)self;
    int ccount = vg->children.size;
    if (ccount == 0) {
        return;
    }
    
    int widthX = vg->view.width / ccount;
    int x = 0;
    VIEW_OR_GROUP* node = head_VIEW_OR_GROUP_list(&vg->children);
    for (int i = 0; i < ccount; i++) {
        if (node->type == TYPE_VIEW) {
            ((VIEW*)node->child)->x = x;
            ((VIEW*)node->child)->y = 0;
            ((VIEW*)node->child)->width = widthX;
            ((VIEW*)node->child)->height = vg->view.height;
        } else if (node->type == TYPE_VIEW_GROUP) {
            ((VIEW_GROUP*)node->child)->view.x = x;
            ((VIEW_GROUP*)node->child)->view.y = 0;
            ((VIEW_GROUP*)node->child)->view.width = widthX;
            ((VIEW_GROUP*)node->child)->view.height = vg->view.height;
        }
        node = next_VIEW_OR_GROUP_list(node);
        x += widthX;
    }
}

void vertical_fitted_linear_layout(void* self) {
    VIEW_GROUP* vg = (VIEW_GROUP*)self;
    int ccount = vg->children.size;
    if (ccount == 0) {
        return;
    }

    int heightX = vg->view.height / ccount;
    int y = 0;
    VIEW_OR_GROUP* node = head_VIEW_OR_GROUP_list(&vg->children);
    for (int i = 0; i < ccount; i++) {
        if (node->type == TYPE_VIEW) {
            ((VIEW*)node->child)->x = 0;
            ((VIEW*)node->child)->y = y;
            ((VIEW*)node->child)->width = vg->view.width;
            ((VIEW*)node->child)->height = heightX;
        } else if (node->type == TYPE_VIEW_GROUP) {
            ((VIEW_GROUP*)node->child)->view.x = 0;
            ((VIEW_GROUP*)node->child)->view.y = y;
            ((VIEW_GROUP*)node->child)->view.width = vg->view.width;
            ((VIEW_GROUP*)node->child)->view.height = heightX;
        }
        node = next_VIEW_OR_GROUP_list(node);
        y += heightX;
    }
}

void three_columns_layout(void* self) {
    VIEW_GROUP* vg = (VIEW_GROUP*)self;
    int ccount = vg->children.size;
    if (ccount == 0) {
        return;
    }
    int extra = ccount % 3 == 0 ? 0 : 1;
    int its = ccount / 3;
    int width = vg->view.width;
    int height = vg->view.height;
    int width3 = width / 3;
    int heightX = height / (its + extra);

    VIEW_OR_GROUP* node = head_VIEW_OR_GROUP_list(&vg->children);
    for (int i = 0; i < its; i++) {
        int x = 0;
        for (int j = 0; j < 3; j++) {
            if (node->type == TYPE_VIEW) {
                ((VIEW*)node->child)->x = x;
                ((VIEW*)node->child)->y = heightX*i;
                ((VIEW*)node->child)->width = width3;
                ((VIEW*)node->child)->height = heightX * (i + 1);
            } else if (node->type == TYPE_VIEW_GROUP) {
                ((VIEW_GROUP*)node->child)->view.x = x;
                ((VIEW_GROUP*)node->child)->view.y = heightX*i;
                ((VIEW_GROUP*)node->child)->view.width = width3;
                ((VIEW_GROUP*)node->child)->view.height = heightX * (i + 1);
            }
            node = next_VIEW_OR_GROUP_list(node);
            x += width3;
        }
    }
    
    if (extra == 1) {
        int x = 0;
        for (int i = 0; i < ccount % 3; i++) {
            if (node->type == TYPE_VIEW) {
                ((VIEW*)node->child)->x = x;
                ((VIEW*)node->child)->y = heightX * its;
                ((VIEW*)node->child)->width = width3;
                ((VIEW*)node->child)->height = heightX * (its + 1);
            } else if (node->type == TYPE_VIEW_GROUP) {
                ((VIEW_GROUP*)node->child)->view.x = x;
                ((VIEW_GROUP*)node->child)->view.y = heightX*its;
                ((VIEW_GROUP*)node->child)->view.width = width3;
                ((VIEW_GROUP*)node->child)->view.height = heightX * (its + 1);
            }
            x += width3;
            node = next_VIEW_OR_GROUP_list(node);
        }
    }
}