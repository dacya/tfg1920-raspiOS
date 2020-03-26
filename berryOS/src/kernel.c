#include <stddef.h>
#include <stdint.h>

#include <io/uart.h>
#include <interrupts.h>
#include <io/gpu.h>
#include <local_timer.h>
#include <io/gpio.h>
#include <proc/pcb.h>

extern void io_halt();


void saluda(void){
    int a = 2;
    int i = 0;
    while(1){
        if(i == 100000000){
            uart_puts("I'm the CREATED function process --> a = ");
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
    
    /* UART */
    uart_init();
    uart_puts("Hello, World!\r\n\r\n");

    /* INTERRUPTS */
    uart_puts(">> Interrupts init: ");
    interrupts_init();
    uart_puts(" [OK] \r\n");

    uart_puts(">> Register timer handler and clearer: ");
    register_irq_handler(ARM_TIMER, local_timer_handler, local_timer_clearer);
    uart_puts(" [OK] \r\n");

    /* DYNAMIC MEMORY */
    uart_puts(">> Dynamic memory: ");
    mem_init(((atag_t *)atags));
    uart_puts(" [OK] \r\n");  

    /* HDMI */
    uart_puts(">> GPU init: ");
    gpu_init();
    uart_puts(" [OK] \r\n");

    /* Processes */
    uart_puts(">> Processes init: ");
    process_init();
    uart_puts(" [OK] \r\n");

    /* LOCAL TIMER */
    uart_puts(">> Local timer init: ");
    local_timer_init(VIRTUAL_SYS, 1000);
    uart_puts(" [OK] \r\n");

    //TEST PROCESS SECTION
    create_kernel_thread(&saluda, "Proc1", 5);
}