#include <stddef.h>
#include <stdint.h>

#include <io/uart.h>
#include <proc/pcb.h>
#include <utils/stdlib.h>

void cuenta10(){
    int i;
    for(i = 0; i < 10; i++){
        uart_puts(itoa(i));
        uart_putc('\n');
        schedule();
    }
    uart_puts("FIN --> ");
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{   
    (void) r0;
    (void) r1;
    (void) atags;
    
    int i;
    for(i = 0; i < 10; i++){
        uart_puts("Main --> ");
        uart_puts(itoa(i));
        uart_putc('\n');

        if(i == 2)
            create_kernel_thread(cuenta10, "proc1", 5);
        if(i == 5)
            create_kernel_thread(cuenta10, "proc2", 5);
        if(i == 8)
            create_kernel_thread(cuenta10, "proc3", 5);
        schedule();
    }
    
    uart_puts("FIN --> main");
    
    while(1);
}

