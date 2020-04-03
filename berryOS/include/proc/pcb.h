/**
 * 
 * Used jsandler tutorial as example
 * https://jsandler18.github.io/tutorial/process.html
 * 
 * Raúl Sánchez Montaño
 * 
 * Feb, 9, 2020
 * 
 * Fixed logic problems with jsandler tutorial and changed the
 * pcb struc.
 * 
 * Added support to register schedulers functions. The FCFS and FIFO
 * are left to next students to implement them.
 * 
 * Alejandro Cancelo Correia
 * April, 03, 2020
*/

#ifndef PCB_H
#define PCB_H

#include <stdint.h>
#include <utils/list.h> 

#define MAX_SCHEDULERS 4

/**
 * A simple type to denote kernel thread functions
 */
typedef void (*kthread_function_f)(void);

/**
 * A simple type to denote a pointer to a scheduler functions.
 * Those functions will apply a scheduling policy
 */
typedef void (*kscheduling_function_f)(void);

typedef struct {
    uint32_t cpsr; //(Saved Process State Register)
    uint32_t* lr; //pointer to return address
    uint32_t* pc;
    uint32_t r12;
    uint32_t r11;
    uint32_t r10;
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
} proc_saved_state_t;

typedef struct pcb {
    /*
    Pointer to corresponding stack for this process. At first it will be initialized with a pointer
    to proc_saved_state_t but after the first scheduling process, that pointer will be changed to
    a correct value
    */
    uint32_t* stack_pointer_to_saved_state;
    void* stack_page; //The stack for this proces.  The stack starts at the end of this page
    uint32_t pid; //The process ID number
    DEFINE_LINK(pcb);
    char proc_name[20]; //The process's name
} process_control_block_t;

DEFINE_LIST(pcb);
DEFINE_HEADERS(pcb);


typedef enum {
    FIFO = 0,
    RR = 1,
    FCFS = 2,
    OTHER = 3
} sched_type_t;

typedef struct {
    kscheduling_function_f sched_function;
    char registered;
} sched_f_control_t;

typedef struct  {
    kscheduling_function_f by_default;
    sched_f_control_t schedulers[MAX_SCHEDULERS];
    sched_type_t using;
} sched_control_t;

/**
 * Initialize and allocate the pcb,
 * including the list of processes that
 * want to run first.
 */
void process_init(void);

/**
 * Switch between the current process and the first
 * process in the queue
 */
void schedule(void);

/**
 * Create a new process and add it to the queue
 * 
 * @param thread_func The address to the function that will be the new process 
 * @param name The new process name
 * @param name_size The new process name size
 */
void create_kernel_thread(kthread_function_f thread_func, char * name, int name_size);

/**
 * This function will register an scheduler function of the specified type. The 
 * specified type must not have a function already registered
 * 
 * @param sched_func A pointer to the scheduler function to register
 * @param policy_type Type of the scheduling policy.
 *      The only accepted types are:
 *          RR - Round Robin
 *          FIFO - First In First Out
 *          FCFS - First Come First Server
 *          OTHER - User specific function to use
 * @return It will retun 0 if the function could register the policy or -1 if i could not
 * @see sched_type_t
 * @see unregister_scheduler_policy
 */
int register_scheduler_policy(kscheduling_function_f sched_func, sched_type_t policy_type);

/**
 * This function will delete the kscheduling_function_f marke for a specified policy type.
 * This function always works.
 * 
 * @param policy_type Type of the scheduling policy
 * @see sched_type_t
 * @see register_scheduler_policy
*/
void unregister_scheduler_policy(sched_type_t policy_type);

/**
 * Change policy to one of the accepted types. The policy to change must have a
 * kscheduling_function_f registered
 * 
 * @param policy_type Type to change
 * @return it will return 0 if the change was possible or -1 if it wasn't
 * @see register_scheduler_policy
 * @see sched_type_t
*/
int change_scheduling_policy(sched_type_t policy_type);
void print_processes();


#endif
