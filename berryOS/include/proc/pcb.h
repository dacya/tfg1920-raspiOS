/**
 * This library contains the pcb structure.
 * 
 * Used jsandler tutorial as example
 * https://jsandler18.github.io/tutorial/process.html
 * 
 * Raúl Sánchez Montaño
 * 
 * Feb, 9, 2020
*/
/*
#ifndef PCB_H
#define PCB_H

#include <stdint.h>
#include <utils/list.h> 

typedef struct {
    uint32_t r0;
    uint32_t r1; 
    uint32_t r2; 
    uint32_t r3; 
    uint32_t r4; 
    uint32_t r5; 
    uint32_t r6; 
    uint32_t r7; 
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t cpsr; 
    uint32_t sp;
    uint32_t lr;
} proc_saved_state_t;

typedef struct pcb {
    proc_saved_state_t * saved_state; // Pointer to where on the stack this process's state is saved. Becomes invalid once the process is running
    void * stack_page;                // The stack for this proces.  The stack starts at the end of this page
    uint32_t pid;                     // The process ID number
    DEFINE_LINK(pcb);
    char proc_name[20];               // The process's name
} process_control_block_t;


/**
 *  Initialize and allocate the pcb,
 *  including the list of processes that
 *  want to run first.
 *
void process_init(void);

#endif
*/