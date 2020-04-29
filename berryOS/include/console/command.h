#ifndef _COMMAND_H
#define _COMMAND_H

#include <utils/list.h>
#include <utils/unused.h>

typedef struct COMMAND {
    char* key;
    char* helpText;
    void (*trigger)(int argc, char** argv);
} COMMAND;

typedef struct comm_wrapper {
    COMMAND* comm;
    DEFINE_LINK(comm_wrapper);
} comm_wrapper;

DEFINE_LIST(comm_wrapper);
DEFINE_HEADERS(comm_wrapper);

void init_commands();
void regcomm(COMMAND* comm);
void unregcomm(COMMAND* command);
void commatch(char* match, int argc, char** argv);

#endif /* _COMMAND_H */