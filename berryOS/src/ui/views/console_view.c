#include <ui/views/console_view.h>
#include <ui/layouts.h>

VIEW_GROUP consoleView;
VIEW lineSeparator;
VIEW textInput;
VIEW display;

void init_console(int width, int height) {
    new_view_group(&consoleView, width, height, 0, 0, vertical_linear_layout);
    
    new_view(&display, width, (height - 1) * (100 - CONSOLE_INPUT_HEIGHT) / 100, 0, 0);
    addView(&consoleView, &display);

    new_view(&lineSeparator, width, 1, 0, 0);
    lineSeparator.bgColor = WHITE;
    addView(&consoleView, &lineSeparator);

    new_view(&textInput, width, height * CONSOLE_INPUT_HEIGHT / 100, 0, 0);
    textInput.bgColor = RED;
    addView(&consoleView, &textInput);
    textInput.text = "> ";
}
