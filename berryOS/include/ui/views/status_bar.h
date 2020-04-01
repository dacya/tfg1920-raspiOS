#ifndef _STATUS_BAR_H
#define _STATUS_BAR_H

#include <ui/view_group.h>

#define WIDTH_PERCENTAGE 5
#define BG_COLOR GREY

extern VIEW_GROUP statusBarView;

void init_status_bar(int width, int height);

#endif /* _STATUS_BAR_H */