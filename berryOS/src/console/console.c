#include <console/console.h>
#include <io/uart.h>
#include <io/stdio.h>
#include <ui/views/console_view.h>
#include <mem/mem.h>
#include <proc/pcb.h>
#include <utils/stdlib.h>
#include <console/command.h>
#include <utils/unused.h>

COMMAND cls;

int MAX_CONSOLE_LINE_INPUT_SIZE = 20;
const char EOL = '\n';
const char CR = '\r';

int parse_args(char* str, char*** argv) {
    int argc = 1, new = 1, i = 0;
    char c;
    while ((c = str[i]) != '\0') {  // count all words to save mem
        if (c == ' ') {
            argc++;
        }
        i++;
    }
    *argv = kmalloc(argc * sizeof(char*));
    argc = 0;
    i = 0;
    while ((c = str[i]) != '\0') {  // fill argv
        if (new) {
            (*argv)[argc] = &str[i];
            argc++;
            new = 0;
        } else if (c == ' ') {
            str[i] = '\0';
            new = 1;
        }
        i++;
    }
    
    return argc;
}

void read_proc(void) {
    char* comm = kmalloc(MAX_CONSOLE_LINE_INPUT_SIZE + 2);
    char c;
    int size = 0;
    int flag_over = 0;
    do {
        c = readChar();
        switch (c) {
            case '\n': //changed: constant EOL
            case '\r': //changed: constant CR 
                comm[size] = '\0';
                if (size > 0) {
                    printLn("elwenquesito"); //funca
                    print("resultado ");
                    enrichedPrintLn(comm, &GREEN, NULL);
                    char** argv;
                    int argc = parse_args(comm, &argv);
                    uart_hex_puts((uint32_t)comm);
                    commatch(argv[0], argc - 1, argc < 2 ? NULL : &argv[1]);
                    // commatch(comm, 0, NULL);
                    size = 0;
                    clear_input();
                    kfree(argv);
                } else {
                    printLn("$ ");
                }
                break;
            case 127: // del
                if (size > 0) {
                    if (flag_over) {
                        del_char();
                        flag_over = 0;
                    }
                    comm[--size] = '\0';
                    del_char();
                }
                break;
            default:
                if (size < MAX_CONSOLE_LINE_INPUT_SIZE) {
                    comm[size++] = c;
                    comm[size+ 1] = '\0';
                    uart_hex_puts((uint32_t)comm);
                    uart_putln(comm);
                    put_char(c);
                } else {
                    if (flag_over) {
                        del_char();
                        put_char(1);
                    } else {
                        put_char(1);
                        flag_over = 1;
                    }
                }
                break;
        }
    } while (1);
}

void comm_clear_screen(int argc, char** argv) {
    MARK_UNUSED(argc);
    MARK_UNUSED(argv);
    console_clear_screen();
}

void start_console() {
    cls.helpText = "Clears the screen";
    cls.key = "cls";
    cls.trigger = comm_clear_screen;
    regcomm(&cls);
 
    create_kernel_thread(read_proc, "uart_console_input", 19);
    // read_proc();
}