#include <console/console.h>
#include <io/uart.h>
#include <io/stdio.h>
#include <ui/views/console_view.h>
#include <mem/mem.h>

int MAX_CONSOLE_LINE_INPUT_SIZE = 256;
const char EOL = '\n';
const char CR = '\r';

void start_console() {
    char* comm = kmalloc(MAX_CONSOLE_LINE_INPUT_SIZE);
    char c;
    int size = 0;
    do {
        c = readChar();
        //uart_putln(itoa((int)c));
        switch (c) {
            case EOL:
            case CR:
                size = 0;
                clear_input();
                break;
            case 127: // del
                if (size > 0) {
                    comm[--size] = '\0';
                    del_char();
                }
                break;
            default:
                if (size < MAX_CONSOLE_LINE_INPUT_SIZE) {
                    comm[size++] = c;
                    put_char(c);
                    break;
                }
        }
    } while (1);
}