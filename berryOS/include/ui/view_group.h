#ifndef _VIEW_GROUP_H
#define _VIEW_GROUP_H

#include <ui/view.h>
#include <utils/list.h>

DEFINE_LIST(VIEW);
DEFINE_LINK(VIEW);

typedef struct {
    VIEW view;
    VIEW_list_t* children;
} VIEW_GROUP;

VIEW_GROUP new_view_group(int width, int height, int x, int y);
void addView(VIEW view);
void removeView(VIEW view);
void draw_group(VIEW_GROUP v);

#endif /* _VIEW_GROUP_H */