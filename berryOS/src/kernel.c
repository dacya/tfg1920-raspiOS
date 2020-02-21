#include <stddef.h>
#include <stdint.h>

#include <io/uart.h>
#include <interrupts.h>
#include <io/gpu.h>
#include <local_timer.h>

extern void io_halt();

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {   
    (void) r0;
    (void) r1;
    (void) atags;

    /* UART */
    uart_init();

    /* INTERRUPTS */
    local_timer_init();
    interrupts_init();
    //register_irq_handler(ARM_TIMER, local_timer_handler, local_timer_clearer);

    uart_puts("Hello, World!\r\n");
  
    /* DYNAMIC MEMORY */
    mem_init(((atag_t *)atags));
    
    /* HDMI */
    gpu_init();  
  
    while (1) {
        io_halt();
    }
}
