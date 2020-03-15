#ifndef _VIEW_GROUP_H
#define _VIEW_GROUP_H

#include <ui/view.h>
#include <utils/list.h>

typedef enum vtype {
    TYPE_VIEW,
    TYPE_VIEW_GROUP
} vtype;

typedef struct VIEW_OR_GROUP {
    void* child;
    vtype type;
    DEFINE_LINK(VIEW_OR_GROUP);
} VIEW_OR_GROUP;

DEFINE_LIST(VIEW_OR_GROUP);
DEFINE_HEADERS(VIEW_OR_GROUP);

typedef struct VIEW_GROUP {
    VIEW view;
    VIEW_OR_GROUP_list_t children;
    void (*layout)(void*);
} VIEW_GROUP;

void new_view_group(VIEW_GROUP* vg, int width, int height, int x, int y, void(*layout)(void*));
void addView(VIEW_GROUP* vg, VIEW* view);
void addViewGroup(VIEW_GROUP* vg, VIEW_GROUP* view);
void removeView(VIEW_GROUP* vg, VIEW* view);
void removeViewGroup(VIEW_GROUP* vg, VIEW_GROUP* view);
void* removeViewByIndex(VIEW_GROUP* vg, int index);
VIEW_OR_GROUP* getViewByIndex(VIEW_GROUP* vg, int index);
void drawGroup(VIEW_GROUP* v);
//void drawRelative(VIEW_GROUP*vg, VIEW* v);

#endif /* _VIEW_GROUP_H */