#include <stddef.h>
#include <stdint.h>

#include <interrupts.h>
#include <local_timer.h>
#include <io/gpio.h>
#include <io/uart.h>
#include <proc/pcb.h>
#include <io/stdio.h>
#include <mem/mem.h>
#include <io/gpu.h>

extern void io_halt();


void saluda(){
    int a = 2;
    int i = 0;
    while(1){
        if(i == 10000000){
            uart_puts("I'm the CREATED function process --> a = ");
            uart_putln(itoa(a));
            i = 0;
        }
       i++;
    }
}

void saluda2(){
    int a = 3;
    int i = 0;
    while(1){
        if(i == 10000000){
            uart_puts("I'm the second CREATED function process --> a = ");
            uart_putln(itoa(a));
            i = 0;
            break;
        }
       i++;
       
    }
}
/*
------------\
            ---------\
                      -----------/
*/

void main_function_process_test(){
    int a = 1;
    int i = 0;    
    while (1) {
        if(i == 10000000){
            uart_puts("I'm the main function process --> a = ");
            uart_putln(itoa(a));
            i = 0;
        }
       i++;    
    }
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {   
    (void) r0;
    (void) r1;
    (void) atags;
    pin_set_function(17, OUTPUT);

    /* DYNAMIC MEMORY */
    uart_puts(">> Dynamic memory: ");
    mem_init(((atag_t *)atags));
    uart_putln(" [OK]");  

    /* HDMI */
    gpu_init();

    /* UART */
    uart_init();
    printLn("Hello world!");

    /* INTERRUPTS */
    printLn(">> Interrupts init: ");
    interrupts_init();
    enrichedPrint(" [OK]", &GREEN, NULL);
    
    printLn(">> Local timer init: ");
    local_timer_init(VIRTUAL_SYS, 2000);
    enrichedPrint(" [OK]", &GREEN, NULL);

    printLn(">> Register timer handler & clearer: ");
    register_irq_handler(ARM_TIMER, local_timer_handler, local_timer_clearer);
    enrichedPrint(" [OK]", &GREEN, NULL);

    /* Processes */
    printLn(">> Processes init: ");
    process_init();
    enrichedPrint(" [OK]", &GREEN, NULL);

    create_kernel_thread(saluda, "Proc1", 5);
    create_kernel_thread(saluda2, "Proc2", 5);
    
    print_processes();

    // TEST PROCESS SECTION
    main_function_process_test();
    //while (1){}
}