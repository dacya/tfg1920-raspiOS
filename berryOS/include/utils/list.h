/**
 * This library contains a linked list
 * defined with macros.
 *
 * Obtained in 
 * https://github.com/jsandler18/raspi-kernel/tree/bd5026277370360fb04f04f5b050c6853466c86d
 *  
 * Raúl Sánchez Montaño
 * 
 * Dic 16, 2019
 */



#ifndef LIST_H
#define LIST_H

#include <stddef.h>

#define DEFINE_LIST(nodeType)       \
typedef struct nodeType##list {     \
    struct nodeType ghost;          \
    struct nodeType* pghost;        \
    uint32_t size;                  \
} nodeType##_list_t;

#define DEFINE_LINK(nodeType)       \
struct nodeType * next##nodeType;   \
struct nodeType * prev##nodeType;

#define INITIALIZE_LIST(list)           \
    list.size = 0;

#define INITIALIZE_LIST2(list, nodeType)           \
    list.size = 0;\
    list.ghost.next##nodeType = &list.ghost; \
    list.ghost.prev##nodeType = &list.ghost; \
    list.pghost = &list.ghost;

#define IMPLEMENT_LIST(nodeType)                                                    \
void append_##nodeType##_list(nodeType##_list_t * list, struct nodeType * node) {   \
    node->prev##nodeType = list->ghost.prev##nodeType;                              \
    list->ghost.prev##nodeType->next##nodeType = node;                              \
    list->ghost.prev##nodeType = node;                                              \
    node->next##nodeType = &list->ghost;                                            \
    list->size += 1;                                                                \
}                                                                                   \
                                                                                    \
void push_##nodeType##_list(nodeType##_list_t * list, struct nodeType * node) {     \
    node->next##nodeType = list->ghost.next##nodeType;                              \
    node->prev##nodeType = NULL;                                                    \
    list->ghost.next##nodeType = node;                                              \
    list->size += 1;                                                                \
}                                                                                   \
                                                                                    \
struct nodeType * peek_##nodeType##_list(nodeType##_list_t * list) {                \
    return list->ghost.next##nodeType;                                              \
}                                                                                   \
                                                                                    \
struct nodeType * pop_##nodeType##_list(nodeType##_list_t * list) {                 \
    if(list->size == 0)                                                             \
        return NULL;                                                                \
    struct nodeType * res = list->ghost.next##nodeType;                             \
    list->ghost.next##nodeType = res->next##nodeType;                               \
    res->next##nodeType->prev##nodeType = NULL;                                     \
    list->size -= 1;                                                                \
    return res;                                                                     \
}                                                                                   \
                                                                                    \
uint32_t size_##nodeType##_list(nodeType##_list_t * list) {                         \
    return list->size;                                                              \
}                                                                                   \
                                                                                    \
struct nodeType * next_##nodeType##_list(struct nodeType * node) {                  \
    return node->next##nodeType;                                                    \
}                                                                                   \
                                                                                    \
struct nodeType * prev_##nodeType##_list(struct nodeType * node) {                  \
    return node->prev##nodeType;                                                    \
}                                                                                   \
                                                                                    \
struct nodeType * head_##nodeType##_list(nodeType##_list_t * list) {                \
    return list->ghost.next##nodeType;                                              \
}                                                                                   \
                                                                                    \
struct nodeType * tail_##nodeType##_list(nodeType##_list_t * list) {                \
    return list->ghost.prev##nodeType;                                              \
}                                                                                   \
                                                                                    \
void remove_##nodeType##_list(nodeType##_list_t * list, struct nodeType * node) {   \
    struct nodeType* init = list->ghost.next##nodeType;                             \
    while (init != NULL) {                                                          \
        if (init == node) {                                                         \
            init->prev##nodeType->next##nodeType = init->next##nodeType;            \
            init->next##nodeType->prev##nodeType = init->prev##nodeType;            \
            list->size -= 1;                                                        \
            return;                                                                 \
        }                                                                           \
    }                                                                               \
}  

#endif
