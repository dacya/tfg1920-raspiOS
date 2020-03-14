#include <proc/pcb.h>
#include <mem/mem.h>
#include <utils/stdlib.h>
#include <interrupts.h>
#include <io/uart.h>

static uint32_t pids = 1;
#define NEW_PID pids++;

extern uint8_t __end;
extern void switch_to_thread(process_control_block_t * old, process_control_block_t * new);

IMPLEMENT_LIST(pcb);

process_control_block_t * current_process;

pcb_list_t run_queue;
//pcb_list_t all_proc_list;

void process_init(void){
    process_control_block_t * main_pcb;
    INITIALIZE_LIST(run_queue, pcb);
    //INITIALIZE_LIST(all_proc_list, pcb);

    uart_puts("Run queue -->");
    uart_puts(itoa((int)&run_queue));
    uart_putc('\n');
    

    //Allocate and initialize the block 
    main_pcb = kmalloc(sizeof(process_control_block_t));
    main_pcb->pid = NEW_PID;
    main_pcb-> stack_page = (void *)&__end;
    memcpy(main_pcb->proc_name, "Init", 5*sizeof(char));

    //Add pcb to all process list and set it as the current process
    
   
    //append_pcb_list(&all_proc_list, main_pcb);
    
    
 
    
    current_process = main_pcb;
    
    //Start timer with 10 ms

    //timer_set(10000);
}

void schedule(void){
    DISABLE_INTERRUPTS();
    process_control_block_t* new_thread;
    process_control_block_t* old_thread;

    uart_putln("size_pcb comparison");
    //If run_queue is empty, the current process continue
    if(size_pcb_list(&run_queue) == 0){
        ENABLE_INTERRUPTS();
        uart_putln("inside the if size_pcb comparison");
        return;
    }
    
    //If is not empty, save the current process and pop the first process in the run_queue.
    //This replacement method works for RR and FIFO.
    uart_putln("new_thread pop");
    new_thread = pop_pcb_list(&run_queue);
    uart_putln("old_thread assign");
    old_thread = current_process;
    uart_putln("current process");
    current_process = new_thread;

    uart_putln("append");
    append_pcb_list(&run_queue, old_thread); 
    
    //uart_puts(new_thread->proc_name);
    //uart_puts(" --> ");
    uart_putln("switch_to_thread");
    uart_hex_puts((uint32_t)old_thread);
    uart_hex_puts((uint32_t)new_thread);
    //Switch the processes contexts
    switch_to_thread(old_thread, new_thread);
    ENABLE_INTERRUPTS();
}

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
    switch_to_thread(old_thread, new_thread);
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
    pcb->proc_name[MIN(name_size, 19) + 1] = '\0'; //changed last character
        
    //Calculate the location of the stack pointer
    new_proc_state = pcb->stack_page + PAGE_SIZE - sizeof(proc_saved_state_t);
    pcb->saved_state = new_proc_state;
    
     // Set up the stack that will be restored during a context switch
    bzero2(new_proc_state, sizeof(proc_saved_state_t));
    new_proc_state->lr = (uint32_t)thread_func;     // lr is used as return address in switch_to_thread
    new_proc_state->sp = (uint32_t)reap;            // When the thread function returns, this reaper routine will clean it up
    new_proc_state->cpsr = 0x13 | (8 << 1);         // Sets the thread up to run in supervisor mode with irqs only
    
    // add the thread to the lists
    
    
    //append_pcb_list(&all_proc_list, pcb);
    append_pcb_list(&run_queue, pcb);

       
    

    /*
    uart_puts("Create -->");
    uart_puts(itoa((int)pcb));
    uart_putc('\n');
    */ 

    /*
    uart_puts("\nCreado ");
    uart_puts(pcb->proc_name);
    uart_puts(" con PID ");
    uart_puts(itoa(pcb->pid));
    uart_puts(", hay ");
    uart_puts(itoa(run_queue.size));
    uart_puts(" procesos en la run_queue\n\n");
    */
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
