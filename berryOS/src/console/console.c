#include <console/console.h>
#include <io/uart.h>
#include <io/stdio.h>
#include <ui/views/console_view.h>
#include <mem/mem.h>
#include <proc/pcb.h>
#include <utils/stdlib.h>
#include <console/command.h>

int MAX_CONSOLE_LINE_INPUT_SIZE = 256;
const char EOL = '\n';
const char CR = '\r';

void read_proc(void) {
    char* comm = kmalloc(MAX_CONSOLE_LINE_INPUT_SIZE);
    char c;
    int size = 0;
    do {
        c = readChar();
        //uart_putln(itoa((int)c));
        switch (c) {
            case EOL:
            case CR:
                comm[size] = '\0';
                print("$ ");
                enrichedPrintLn(comm, &GREEN, NULL);
                commatch(comm, 0, NULL);
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

void start_console() {
    create_kernel_thread(read_proc, "uart_console_input", 19);
    //read_proc();
}