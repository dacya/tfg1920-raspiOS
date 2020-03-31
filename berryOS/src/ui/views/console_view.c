#include <ui/views/console_view.h>
#include <ui/layouts.h>
#include <mem/mem.h>
#include <utils/fonts/chars_pixel.h>
#include <utils/stdlib.h>
#include <io/uart.h>

VIEW_GROUP consoleView;
VIEW lineSeparator;
VIEW textInput;
VIEW_GROUP display;

int LINE_HEIGHT;
int MAX_LINES;
int MAX_CHAR;

int CONSOLE_VIEW_FONT_SIZE = 2;

void init_console(int width, int height) {
    new_view_group(&consoleView, width, height, 0, 0, vertical_linear_layout);
    
    new_view_group(&display, width, (height - 1) * (100 - CONSOLE_INPUT_HEIGHT) / 100, 0, 0, vertical_linear_layout);
    display.view.bgColor = BLACK;
    display.view.fontSize = CONSOLE_VIEW_FONT_SIZE;
    addViewGroup(&consoleView, &display);

    new_view(&lineSeparator, width, 1, 0, 0);
    lineSeparator.bgColor = WHITE;
    addView(&consoleView, &lineSeparator);

    new_view(&textInput, width, (height - 1) * CONSOLE_INPUT_HEIGHT / 100, 0, 0);
    textInput.bgColor = RED;
    addView(&consoleView, &textInput);
    textInput.text = "> ";

    LINE_HEIGHT = display.view.fontSize * CHAR_MIN_SIZE + CHAR_MIN_SIZE;
    MAX_LINES = display.view.height / LINE_HEIGHT;
    MAX_CHAR = display.view.width / (display.view.fontSize * CHAR_MIN_SIZE);

    put_char('h');
    put_char('h');
    put_char('h');
    put_char('h');
    put_char('h');

}

char* getOverFlow(char* str, int filled) {
    int left = MAX_CHAR - filled;

    int strsize = strlen(str);

    if (strsize <= left)
        return NULL;

    int str_left = strlen(str + left) + 1; // add 1 extra for the \0
    char* newstr = kmalloc(str_left);

    memcpy(newstr, str + left, str_left);
    return newstr;
}

void console_putLn(char* str, color_24* textColor, color_24* bgColor) {
    VIEW_GROUP* line;
    int update = 0;
    if (display.children.size < MAX_LINES) {
        line = kmalloc(sizeof(VIEW_GROUP));
        new_view_group(line, display.view.width, LINE_HEIGHT, 0, 0, horizontal_linear_layout);
    } else {
        line = removeViewByIndex(&display, 0);
        while (line->children.size > 0) {
            VIEW* v = removeViewByIndex(line, 0);
            destroyView(v);
            kfree(v);
        }
        update = 1;
    }
    addViewGroup(&display, line);
    
    VIEW* string = kmalloc(sizeof(VIEW));
    new_view(string, CHAR_MIN_SIZE*display.view.fontSize*strlen(str), LINE_HEIGHT, 0, 0);

    string->textColor = textColor == NULL ? display.view.textColor : *textColor;
    string->bgColor = bgColor == NULL ? display.view.bgColor : *bgColor;
    string->fontSize = display.view.fontSize;
    string->padding = 0;

    char* oflow = getOverFlow(str, 0);
    setText(string, str);
    addView(line, string);
    if (update) {
        drawGroup(&display);
    } else {
        layoutGroup(&display);
        drawGroup(line);
    }

    if (oflow != NULL) {
        console_putLn(oflow, textColor, bgColor);
        kfree(oflow);
    }
}

int getStringSize(VIEW_GROUP* vg) {
    int size = 0;
    for (int i = 0; i < vg->children.size; i++) {
        size += strlen(((VIEW*)getViewByIndex(vg, i)->child)->text);
    }

    return size;
}

void console_putStr(char* str, color_24* textColor, color_24* bgColor) {
    VIEW_GROUP* line;
    if (display.children.size == 0) {
        console_putLn(str, textColor, bgColor);
        return;
    }
    
    line = getViewByIndex(&display, display.children.size - 1)->child;
    
    VIEW* string = kmalloc(sizeof(VIEW));
    new_view(string, CHAR_MIN_SIZE*display.view.fontSize*strlen(str), LINE_HEIGHT, 0, 0);
    
    string->textColor = textColor == NULL ? display.view.textColor : *textColor;
    string->bgColor = bgColor == NULL ? display.view.bgColor : *bgColor;
    string->fontSize = display.view.fontSize;
    string->padding = 0;

    int filled = getStringSize(line);
    char* oflow = getOverFlow(str, filled);
    setText(string, str);
    //uart_putln(itoa(filled));
    addView(line, string);
    layoutGroup(line);
    draw(string);

    if (oflow != NULL) {
        console_putLn(oflow, textColor, bgColor);
        kfree(oflow);
    }
}

void put_char(char c) {
    int size = strlen(textInput.text) + 2; // add \0 and the new char
    char* news = kmalloc(size);
    news[size - 1] = '\0';
    news[size - 2] = c;

    kfree(textInput.text);
    textInput.text = news;

    draw(&textInput);
}