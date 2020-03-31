#include <console/console.h>
#include <io/uart.h>
#include <io/stdio.h>
#include <ui/views/console_view.h>
#include <mem/mem.h>

int MAX_CONSOLE_LINE_INPUT_SIZE = 256;
char EON = '\n';

void start_console() {
    char* comm = kmalloc(MAX_CONSOLE_LINE_INPUT_SIZE);
    char c;
    int size = 0;
    do {
        while ((c = readChar()) != EON && size < MAX_CONSOLE_LINE_INPUT_SIZE) {
            comm[size++] = c;
            put_char(c);
        }
        size = 0;
    } while (1);
}