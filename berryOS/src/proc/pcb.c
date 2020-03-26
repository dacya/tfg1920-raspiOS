#include <proc/pcb.h>
#include <mem/mem.h>
#include <utils/stdlib.h>
#include <interrupts.h>
#include <io/uart.h>

static uint32_t pids = 1;
#define NEW_PID pids++;
/**
 * Used by the scheduler and irq s handler
 * DO NOT USE
*/
uint32_t __scheduler_finished;


extern uint8_t __end;
extern void switch_process_context(process_control_block_t * old, process_control_block_t * new);
extern void load_init_process(process_control_block_t* init_pcb);
extern void pointer_test(void* a, void* b);


IMPLEMENT_LIST(pcb);

process_control_block_t * current_process;

process_control_block_t* init_process;

pcb_list_t run_queue;
//pcb_list_t all_proc_list;

static void init_function(void){
    int a = 1;
    int i = 0;    
    while (1) {
        if(i == 100000000){
            uart_puts("I'm INIT --> a = ");
            uart_putln(itoa(a));
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

/**
 * Cleans all presence of a process and performs a context switch with the next
 * process available inside the run queue
*/
static void reap(void){
    DISABLE_INTERRUPTS();
    process_control_block_t * new_thread, * old_thread;

    // If nothing on the run queue, there is nothing to do now. just loop
    while (size_pcb_list(&run_queue) == 0);

    // Get the next thread to run.  For now we are using round-robin
    new_thread = pop_pcb_list(&run_queue);
    old_thread = current_process;
    current_process = new_thread;

    // Free the resources used by the old process.  Technically, we are using dangling pointers here, but since interrupts are disabled and we only have one core, it
    // should still be fine
    free_page(old_thread->stack_page);
    kfree(old_thread);
    

    // Context Switch
    switch_process_context(old_thread, new_thread);
}
/**
 * used by the scheduler
 * DO NOT TOUCH, IT WILL RESULT IN UNEXPECTED BEHAVIOR
*/
uint32_t* __reap_pointer = (uint32_t*)reap;

void process_init(void){
    process_control_block_t * main_pcb;
    proc_saved_state_t* new_proc_state;
    INITIALIZE_LIST(run_queue, pcb);
    //INITIALIZE_LIST(all_proc_list, pcb);

    uart_puts("Run queue -->");
    uart_puts(itoa((int)&run_queue));
    uart_putc('\n');

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

    new_proc_state->lr = (uint32_t*)init_function;
    new_proc_state->cpsr = 0x13; // Sets the thread up to run in supervisor mode with irqs only

    append_pcb_list(&run_queue, main_pcb);
    
    //Add pcb to all process list and set it as the current process
    //append_pcb_list(&all_proc_list, main_pcb);
    init_process = main_pcb;
    current_process = NULL;
}

void schedule(void){
    DISABLE_INTERRUPTS();
    process_control_block_t* new_thread;
    process_control_block_t* old_thread;

    //If run_queue is empty, the current process continue
    if(size_pcb_list(&run_queue) == 0){
        ENABLE_INTERRUPTS();
        return;
    }

    if(current_process == NULL){
        current_process = init_process;
        load_init_process(init_process);
        remove_pcb_immediate(&run_queue, init_process);
        uart_putln("INIT loaded!");
        ENABLE_INTERRUPTS();
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
    uart_putln("processes switched!");
    ENABLE_INTERRUPTS();
}

void create_kernel_thread(kthread_function_f thread_func, char * name, int name_size){
    process_control_block_t* pcb;
    proc_saved_state_t* new_proc_state;

    //Allocate and initialize the pcb block
    pcb = kmalloc(sizeof(process_control_block_t));
    uart_puts("Allocate -->");
    uart_puts(itoa((int)pcb));
    uart_putc('\n');
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

    new_proc_state->lr = (uint32_t*)thread_func;     // lr is used as return address in switch_process_context
    new_proc_state->cpsr = 0x13;         // Sets the thread up to run in supervisor mode with irqs only

    // add the thread to the lists
    //append_pcb_list(&all_proc_list, pcb);
    append_pcb_list(&run_queue, pcb);
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
