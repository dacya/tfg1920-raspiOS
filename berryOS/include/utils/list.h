/**
 * This library contains a linked list
 * defined with macros.
 *
 * Based on 
 * https://github.com/jsandler18/raspi-kernel/tree/bd5026277370360fb04f04f5b050c6853466c86d
 *  
 * Raúl Sánchez Montaño - Dic 16, 2019
 * Tomás Golomb Durán - Mar. 04, 2020
 * 
 */

#ifndef LIST_H
#define LIST_H

#include <stddef.h>

#define DEFINE_LIST(nodeType)       \
typedef struct nodeType##list {     \
    struct nodeType ghost;          \
    uint32_t size;                  \
} nodeType##_list_t;

#define DEFINE_LINK(nodeType)       \
struct nodeType * next##nodeType;   \
struct nodeType * prev##nodeType;

#define INITIALIZE_LIST(list, nodeType)           \
    list.size = 0;\
    list.ghost.next##nodeType = &list.ghost; \
    list.ghost.prev##nodeType = &list.ghost;

#define DEFINE_HEADERS(nodeType)                                                    \
void append_##nodeType##_list(nodeType##_list_t * list, struct nodeType * node);    \
void push_##nodeType##_list(nodeType##_list_t * list, struct nodeType * node);      \
struct nodeType * peek_##nodeType##_list(nodeType##_list_t * list);                 \
struct nodeType * pop_##nodeType##_list(nodeType##_list_t * list);                  \
uint32_t size_##nodeType##_list(nodeType##_list_t * list);                          \
struct nodeType * next_##nodeType##_list(struct nodeType * node);                   \
struct nodeType * prev_##nodeType##_list(struct nodeType * node);                   \
struct nodeType * head_##nodeType##_list(nodeType##_list_t * list);                 \
struct nodeType * tail_##nodeType##_list(nodeType##_list_t * list);                 \
void remove_##nodeType##_list(nodeType##_list_t * list, struct nodeType * node);

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
    list->ghost.next##nodeType->prev##nodeType = node;                              \
    node->next##nodeType = list->ghost.next##nodeType;                              \
    node->prev##nodeType = &list->ghost;                                            \
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
    if (list->size == 1)                                                            \
        list->ghost.prev##nodeType = &list->ghost;                                  \
    else                                                                            \
        res->next##nodeType->prev##nodeType = &list->ghost;                         \
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
int has_next_##nodeType##_list(nodeType##_list_t * list, struct nodeType * node) {  \
    return node->next##nodeType != &list->ghost;                                    \
}                                                                                   \
                                                                                    \
int has_prev_##nodeType##_list(nodeType##_list_t * list, struct nodeType * node) {  \
    return node->prev##nodeType != &list->ghost;                                    \
}                                                                                   \
                                                                                    \
void remove_##nodeType##_list(nodeType##_list_t * list, struct nodeType * node) {   \
    struct nodeType* init = list->ghost.next##nodeType;                             \
    if (list->size == 1) {                                                          \
        pop_##nodeType##_list(list);                                                \
        return;                                                                     \
    }                                                                               \
    while (init != &list->ghost) {                                                  \
        if (init == node) {                                                         \
            init->prev##nodeType->next##nodeType = init->next##nodeType;            \
            init->next##nodeType->prev##nodeType = init->prev##nodeType;            \
            list->size -= 1;                                                        \
            return;                                                                 \
        }                                                                           \
        init = init->next##nodeType;                                                \
    }                                                                               \
}  

#endif
