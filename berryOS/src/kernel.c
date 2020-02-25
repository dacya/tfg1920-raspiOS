#include <stddef.h>
#include <stdint.h>

#include <io/uart.h>
#include <interrupts.h>
#include <io/gpu.h>
#include <local_timer.h>
#include <io/gpio.h>


extern void io_halt();

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {   
    (void) r0;
    (void) r1;
    (void) atags;
    pin_set_function(17, OUTPUT);
    
    /* UART */
    //uart_init();
    //uart_puts("Hello, World!\r\n\r\n");

    
    /* DYNAMIC MEMORY */
    //uart_puts(">> Dynamic memory: ");
    mem_init(((atag_t *)atags));
    //uart_puts(" [OK] \r\n");
    
    /* HDMI */
    gpu_init();
    pin_set_as_output(17);

    while (1) {
        //io_halt();
    }
}
/*
* INTERRUPTS *
    uart_puts(">> Local timer init: ");
    local_timer_init();
    uart_puts(" [OK] \r\n");
    
    uart_puts(">> Interrupts init: ");
    interrupts_init();
    uart_puts(" [OK] \r\n");
    
    uart_puts(">> Register timer handler and clearer: ");
    register_irq_handler(ARM_TIMER, local_timer_handler, local_timer_clearer);
    uart_puts(" [OK] \r\n");

*/