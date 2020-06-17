#include <proc/pcb.h>
#include <mem/mem.h>
#include <utils/stdlib.h>
#include <interrupts.h>
#include <io/uart.h>
#include <proc/locks/mutex.h>
#include <console/command.h>
#include <io/stdio.h>
#include <utils/unused.h>

static uint32_t pids = 1;
#define NEW_PID pids++;
/**
 * Used by the scheduler and irq s handler
 * USE OF THIS VARIABLE MAY CAUSE UNEXPECTED BEHAVIOR
 * IN THE SCHEDULER WICH MAY CAUSE A FATAL ERROR
*/
uint32_t __scheduler_finished;

/**
 * Used by the scheduler and irq s handler
 * USE OF THIS VARIABLE MAY CAUSE UNEXPECTED BEHAVIOR
 * IN THE SCHEDULER WICH MAY CAUSE A FATAL ERROR
*/
uint32_t* __process_lr;
/**
 * Used by the scheduler and irq s handler
 * USE OF THIS VARIABLE MAY CAUSE UNEXPECTED BEHAVIOR
 * IN THE SCHEDULER WICH MAY CAUSE A FATAL ERROR
*/
uint32_t* __process_sp;

extern uint8_t __end;
extern void switch_process_context(process_control_block_t * old, process_control_block_t * new);
extern void load_process(process_control_block_t* init_pcb);
extern void yield_to_next_process(process_control_block_t* next_process);
extern void pointer_test(void* a, void* b);


IMPLEMENT_LIST(pcb);

process_control_block_t * current_process;

process_control_block_t* init_process;

pcb_list_t run_queue;
//pcb_list_t all_proc_list;

static sched_control_t* scheduler_control;

extern int mutex;

void print_pcb_stack(process_control_block_t* to_print){
    uint32_t* end_stack = to_print->stack_page + PAGE_SIZE;

    uart_puts("pbc: ");
    uart_puts(to_print->proc_name);
    uart_putln(" ---------------------PRINT PCB STACK---------------------------");
    for(uint32_t* i = to_print->stack_pointer_to_saved_state; i < end_stack; i++){
        uart_puts("stack_memory: ");
        uart_hex_puts((uint32_t)(i));
        uart_puts("value inside:");
        uart_hex_puts(*(i));
        uart_putln("");
    }
    uart_putln("-------------------END PRINT PCB STACK---------------------------");
}

static void init_function(void){
    int i = 0;
    
    while (1) {
        //uart_putln("INIT");
        if(i == 100000000){
            uart_putln("I'm INIT -->");
            i = 0;
        }
       i++;    
    }
}

/**
 * DO NOT TOUCH, JUST FOR DEBUGGINF PURPOSES
 * */
void pointer_test_in_c(void){
    uint32_t* a = (uint32_t*)init_function;
    //void (*function_pointer)() = &init_function;
    int b = 0;

    uart_putln("inside of b");
    uart_hex_puts((uint32_t)b);
    uart_puts("------------------------\n");
    pointer_test((a), &b);
    uart_puts("Value of a:");
    uart_hex_puts((uint32_t)(a));
    uart_puts("Value inside of b:");
    uart_hex_puts((uint32_t)b);
}

static void round_robin_sched_policy(void){
    process_control_block_t* new_thread;
    process_control_block_t* old_thread;

    
    //If run_queue is empty, the current process continue
    if(size_pcb_list(&run_queue) == 0){
        return;
    }

    if(current_process == NULL){
        print_pcb_stack(init_process);
        current_process = init_process;
        load_process(init_process);
        remove_pcb_immediate(&run_queue, init_process);
        uart_putln("INIT loaded!");
        return;
    }

    //If is not empty, save the current process and pop the first process in the run_queue.
    //This replacement method works for RR and FIFO.
    new_thread = pop_pcb_list(&run_queue);
    old_thread = current_process;
    current_process = new_thread;

    append_pcb_list(&run_queue, old_thread); 

    //pointer_test_in_c();

    //Switch the processes contexts
    switch_process_context(old_thread, new_thread);
}

/**
 * Cleans all presence of a process and performs a context switch with the next
 * process available inside the run queue
*/
static void reap(void){
    DISABLE_INTERRUPTS();
    process_control_block_t * new_thread, * old_thread;

    // If nothing on the run queue, there is nothing to do now. just loop
    while (size_pcb_list(&run_queue) == 0);

    // Get the next thread to run.  For now we are using FIFO
    new_thread = pop_pcb_list(&run_queue);
    old_thread = current_process;
    current_process = new_thread;

    //remove_pcb_immediate(&run_queue, old_thread);

    free_page(old_thread->stack_page);
    kfree(old_thread);

    // Context Switch
    yield_to_next_process(new_thread);
}

static void register_process_commands();

void process_init(void){
    register_process_commands();
    process_control_block_t * main_pcb;
    proc_saved_state_t* new_proc_state;
    INITIALIZE_LIST(run_queue, pcb);
    //INITIALIZE_LIST(all_proc_list, pcb);

    //Allocate and initialize the block 
    main_pcb = kmalloc(sizeof(process_control_block_t));
    main_pcb->pid = NEW_PID;
    //we gives INIT it's on stack
    main_pcb->stack_page = alloc_page();
    memcpy(main_pcb->proc_name, "Init", 5*sizeof(char));

    //Calculate the location of the stack pointer
    new_proc_state = main_pcb->stack_page + PAGE_SIZE - sizeof(proc_saved_state_t);
    main_pcb->stack_pointer_to_saved_state = (uint32_t*)new_proc_state;
    
     // Set up the stack that will be restored during a context switch
    bzero2(new_proc_state, sizeof(proc_saved_state_t));

    new_proc_state->r0 = 0x90;
    new_proc_state->r1 = 0x8c;
    new_proc_state->r2 = 0x88;
    new_proc_state->r3 = 0x84;
    new_proc_state->r4 = 0x80;
    new_proc_state->r5 = 0x7c;
    new_proc_state->r6 = 0x78;
    new_proc_state->r7 = 0x74;
    new_proc_state->r8 = 0x70;
    new_proc_state->r9 = 0x6c;
    new_proc_state->r10 = 0x68;
    new_proc_state->r11 = 0x64;
    new_proc_state->r12 = 0x60;

    new_proc_state->pc = (uint32_t*)init_function;
    new_proc_state->lr = (uint32_t*)reap;
    new_proc_state->cpsr = 0x13; // Sets the thread up to run in supervisor mode with irqs only

    append_pcb_list(&run_queue, main_pcb);
    
    //Add pcb to all process list and set it as the current process
    //append_pcb_list(&all_proc_list, main_pcb);
    init_process = main_pcb;
    current_process = NULL;

    sched_control_t* main_scheduling;

    main_scheduling = kmalloc(sizeof(sched_control_t));
    bzero2(main_scheduling, sizeof(sched_control_t));

    main_scheduling->using = RR;

    for(int i = 0; i < MAX_SCHEDULERS; i++){
        main_scheduling->schedulers[i].registered = 0;
    }

    main_scheduling->schedulers[RR].registered = 1;
    main_scheduling->schedulers[RR].sched_function = &round_robin_sched_policy;
    main_scheduling->by_default = &round_robin_sched_policy;

    scheduler_control = main_scheduling;
}

void schedule(void){
    DISABLE_INTERRUPTS();

    if(!scheduler_control->schedulers[scheduler_control->using].registered){
        scheduler_control->by_default();
        ENABLE_INTERRUPTS();
        return;
    }

    scheduler_control->schedulers[scheduler_control->using].sched_function();

    ENABLE_INTERRUPTS();
}

void create_kernel_thread(kthread_function_f thread_func, char * name, int name_size){
    process_control_block_t* pcb;
    proc_saved_state_t* new_proc_state;

    //Allocate and initialize the pcb block
    pcb = kmalloc(sizeof(process_control_block_t));
    pcb->stack_page = alloc_page();
    pcb->pid = NEW_PID;
    memcpy(pcb->proc_name, name, MIN(name_size, 19));
    pcb->proc_name[MIN(name_size, 19)] = '\0'; //changed last character
        
    //Calculate the location of the stack pointer
    new_proc_state = pcb->stack_page + PAGE_SIZE - sizeof(proc_saved_state_t);
    pcb->stack_pointer_to_saved_state = (uint32_t*)new_proc_state;
    
     // Set up the stack that will be restored during a context switch
    bzero2(new_proc_state, sizeof(proc_saved_state_t));

    new_proc_state->r0 = 0x190;
    new_proc_state->r1 = 0x18c;
    new_proc_state->r2 = 0x188;
    new_proc_state->r3 = 0x184;
    new_proc_state->r4 = 0x180;
    new_proc_state->r5 = 0x17c;
    new_proc_state->r6 = 0x178;
    new_proc_state->r7 = 0x174;
    new_proc_state->r8 = 0x170;
    new_proc_state->r9 = 0x16c;
    new_proc_state->r10 = 0x168;
    new_proc_state->r11 = 0x164;
    new_proc_state->r12 = 0x160;

    //We store where the new thread will start execution
    new_proc_state->pc = (uint32_t*)thread_func;
    /* This is the pointer where the new thread will return form it's "main function",
       it will clear all its resources */
    new_proc_state->lr = (uint32_t*)reap;
    //Sets the thread up to run in supervisor mode with irqs only
    new_proc_state->cpsr = 0x13;

    // add the thread to the lists
    //append_pcb_list(&all_proc_list, pcb);
    append_pcb_list(&run_queue, pcb);
}


int register_scheduler_policy(kscheduling_function_f sched_func, sched_type_t policy_type){

    if(scheduler_control->schedulers[policy_type].registered){
        return -1;
    }

    scheduler_control->schedulers[policy_type].registered = 1;
    scheduler_control->schedulers[policy_type].sched_function = sched_func;

    return 0;
}

void unregister_scheduler_policy(sched_type_t policy_type){
    scheduler_control->schedulers[policy_type].registered = 0;
}

int change_scheduling_policy(sched_type_t policy_type){
    if(!scheduler_control->schedulers[policy_type].registered){
        return -1;
    }

    scheduler_control->using = policy_type;

    return 0;
}

void print_processes(){
    
    if(run_queue.size == 0){
        uart_putln("F");
        return;
    }
    process_control_block_t* aux = &run_queue.ghost;
    uart_puts("\r\nSize -->");
    uart_puts(itoa(run_queue.size));
    uart_puts("\n");
        
    while (has_next_pcb_list(&run_queue, aux)){
        aux = next_pcb_list(aux);
        uart_puts("Proceso ");
        uart_puts(aux->proc_name);
        uart_puts(" con PID ");
        uart_puts(itoa(aux->pid));
        uart_puts("\n");
        
    }
    uart_putln("Print end");
    uart_putln("");

}

/* 
----------------------------------------------------
                 Commands functions
----------------------------------------------------
*/

static void console_print_processes(){
    
    if(run_queue.size == 0){
        printLn("Error");
        return;
    }
    process_control_block_t* aux = &run_queue.ghost;
    print("There are ");
    print(itoa(run_queue.size + 1));
    printLn(" processes.");
        
    while (has_next_pcb_list(&run_queue, aux)){
        aux = next_pcb_list(aux);
        print("Process ");
        print(aux->proc_name);
        print(" with PID ");
        printLn(itoa(aux->pid));
    }

    print("Process ");
    print(current_process->proc_name);
    print(" with PID ");
    printLn(itoa(current_process->pid));    
    
}

COMMAND ps;

void ps_function(int argc, char** argv){
    MARK_UNUSED(argc);
    MARK_UNUSED(argv);
    console_print_processes();
    return;
}

static void register_process_commands(){
    ps.helpText = "Print all the system processes";
    ps.key = "ps";
    ps.trigger = ps_function;
    regcomm(&ps);
}