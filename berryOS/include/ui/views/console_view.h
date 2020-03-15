#ifndef _CONSOLE_VIEW_H
#define _CONSOLE_VIEW_H

#include <ui/view_group.h>

#define CONSOLE_INPUT_HEIGHT 10
#define BG_COLOR GREY

extern VIEW_GROUP consoleView;
extern VIEW_GROUP display;

void init_console(int width, int height);
void console_putLn(char* str, color_24* textColor, color_24* bgColor);
void console_putStr(char* str, color_24* textColor, color_24* bgColor);

#endif /* _CONSOLE_VIEW_H */