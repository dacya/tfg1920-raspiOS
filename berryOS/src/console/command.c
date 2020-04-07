#include <console/command.h>
#include <mem/mem.h>
#include <io/stdio.h>
#include <utils/stdlib.h>
#include <utils/unused.h>

IMPLEMENT_LIST(comm_wrapper);

comm_wrapper_list_t commands_list;

COMMAND shcomm;

void shcomm_trigger(int, char**);

void init_commands() {
    INITIALIZE_LIST(commands_list, comm_wrapper);

    // registering shcomm
    shcomm.key = "shcomm";
    shcomm.helpText = "Lists all registered commands.";
    shcomm.trigger = shcomm_trigger;
    regcomm(&shcomm);
}

void regcomm(COMMAND* comm) {
    comm_wrapper* newcomm = kmalloc(sizeof(comm_wrapper));
    newcomm->comm = comm;
    append_comm_wrapper_list(&commands_list, newcomm);
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
    enrichedPrintLn("Available commands", &YELLOW, NULL);
    comm_wrapper* comm = start_iterate_comm_wrapper_list(&commands_list);
    while (has_next_comm_wrapper_list(&commands_list, comm)) {
        comm = next_comm_wrapper_list(comm);
        print_command(comm->comm);
    }
}