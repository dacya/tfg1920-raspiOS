#ifndef _VIEW_GROUP_H
#define _VIEW_GROUP_H

#include <ui/view.h>
#include <utils/list.h>

DEFINE_LIST(VIEW);
DEFINE_HEADERS(VIEW);

typedef struct VIEW_GROUP {
    VIEW view;
    VIEW_list_t children;
    void (*layout)(void*);
} VIEW_GROUP;

void new_view_group(VIEW_GROUP* vg, int width, int height, int x, int y, void(*layout)(void*));
void addView(VIEW_GROUP* vg, VIEW* view);
void removeView(VIEW_GROUP* vg, VIEW* view);
void drawGroup(VIEW_GROUP* v);
void drawRelative(VIEW_GROUP*vg, VIEW* v);

#endif /* _VIEW_GROUP_H */