#ifndef _VIEW_GROUP_H
#define _VIEW_GROUP_H

#include <ui/view.h>

typedef struct {
    view view;
    view* children;
} view_group;

void addView(view view);
void removeView(view view);

#endif /* _VIEW_GROUP_H */