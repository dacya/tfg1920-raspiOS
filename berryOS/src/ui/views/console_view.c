#include <ui/views/console_view.h>
#include <ui/layouts.h>
#include <mem/mem.h>
#include <utils/fonts/chars_pixel.h>
#include <utils/stdlib.h>

VIEW_GROUP consoleView;
VIEW lineSeparator;
VIEW textInput;
VIEW_GROUP display;

int LINE_HEIGHT;
int MAX_LINES;

void init_console(int width, int height) {
    new_view_group(&consoleView, width, height, 0, 0, vertical_linear_layout);
    
    new_view_group(&display, width, (height - 1) * (100 - CONSOLE_INPUT_HEIGHT) / 100, 0, 0, vertical_linear_layout);
    display.view.bgColor = BLACK;
    addViewGroup(&consoleView, &display);

    new_view(&lineSeparator, width, 1, 0, 0);
    lineSeparator.bgColor = WHITE;
    addView(&consoleView, &lineSeparator);

    new_view(&textInput, width, (height - 1) * CONSOLE_INPUT_HEIGHT / 100, 0, 0);
    textInput.bgColor = RED;
    addView(&consoleView, &textInput);
    textInput.text = "> ";

    LINE_HEIGHT = display.view.fontSize * CHAR_MIN_SIZE;
    MAX_LINES = display.view.height / LINE_HEIGHT;
}

void putLn(char* str, color_24 color) {
    VIEW_GROUP* line;
    if (display.children.size < MAX_LINES) {
        line = kmalloc(sizeof(VIEW_GROUP));
        new_view_group(line, display.view.width, LINE_HEIGHT, 0, 0, horizontal_linear_layout);
    } else {
        line = removeViewByIndex(&display, 0);
            while (line->children.size > 0) {
                kfree(removeViewByIndex(line, 0));
        }
    }
    addViewGroup(&display, line);
    
    VIEW* string = kmalloc(sizeof(VIEW));
    new_view(string, CHAR_MIN_SIZE*display.view.fontSize*strlen(str), LINE_HEIGHT, 0, 0);
    string->textColor = color;
    string->bgColor = display.view.bgColor;
    string->fontSize = display.view.fontSize;
    addView(line, string);
    drawGroup(line);
}