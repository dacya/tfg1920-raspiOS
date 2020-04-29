#include <console/command.h>
#include <mem/mem.h>
#include <io/stdio.h>
#include <utils/stdlib.h>
#include <utils/unused.h>

IMPLEMENT_LIST(comm_wrapper);

comm_wrapper_list_t commands_list;

COMMAND shcomm;
COMMAND* delayedList[20];
int delayed = 0;
int initialized = 0;


void shcomm_trigger(int, char**);

void init_commands() {
    INITIALIZE_LIST(commands_list, comm_wrapper);
    initialized = 1;
    for (int i = 0; i < delayed; i++) {
        regcomm(delayedList[i]);
    }
    // registering shcomm
    shcomm.key = "shcomm";
    shcomm.helpText = "Lists all registered commands.";
    shcomm.trigger = shcomm_trigger;
    regcomm(&shcomm);
}

void regcomm(COMMAND* comm) {
    if (initialized) {
        comm_wrapper* newcomm = kmalloc(sizeof(comm_wrapper));
        newcomm->comm = comm;
        append_comm_wrapper_list(&commands_list, newcomm);
    } else {
        if (delayed < 20) {
            delayedList[delayed++] = comm;
        }
    }
}

void unregcomm(COMMAND* command) {
    comm_wrapper* commw = start_iterate_comm_wrapper_list(&commands_list);
    while (has_next_comm_wrapper_list(&commands_list, commw)) {
        commw = next_comm_wrapper_list(commw);
        if (commw->comm == command) {
            remove_comm_wrapper_immediate(&commands_list, commw);
            kfree(commw);
            return;
        }
    }
}

void commatch(char* match, int argc, char** argv) {
    int not_found = 1;
    comm_wrapper* commw = start_iterate_comm_wrapper_list(&commands_list);
    while (has_next_comm_wrapper_list(&commands_list, commw)) {
        commw = next_comm_wrapper_list(commw);
        if (streq(match, commw->comm->key)) {
            commw->comm->trigger(argc, argv);
            not_found = 0;
        }
    }
    if (not_found)
        enrichedPrintLn("command not found", &RED, NULL);
}

void print_command(COMMAND* comm) {
    enrichedPrint(comm->key, &YELLOW, &RED);
    enrichedPrint(" ", NULL, NULL);
    printLn(comm->helpText);
}

void shcomm_trigger(int argc, char** argv) {
    MARK_UNUSED(argc);
    MARK_UNUSED(argv);
    int size = size_comm_wrapper_list(&commands_list);
    comm_wrapper* comm = start_iterate_comm_wrapper_list(&commands_list);
    enrichedPrintLn("Available commands", &YELLOW, NULL);
    for (int i = 0; i < size;) {
        for (int j = 0; j < 3 && i < size; j++) {
            comm = next_comm_wrapper_list(comm);
            print_command(comm->comm);
            i++;
        }
        if (i < size) {
            printLn("");
            enrichedPrint(" Any key ", &BLACK, &GREEN);
            print(" to print more or ");
            enrichedPrint(" Q ", NULL, &RED);
            printLn("to exit ");
            char c = readChar();
            if (c == 'q' || c == 'Q') {
                printLn("Bye.");
                break;
            }
        }
    }
}