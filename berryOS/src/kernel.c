#include <stddef.h>
#include <stdint.h>

#include <io/uart.h>
#include <proc/pcb.h>
#include <utils/stdlib.h>
#include <mem/atag.h>
#include <mem/mem.h>

int j = 0;
void cuenta10(){
    int i;
    for(i = 0; i < 10; i++){
        uart_puts(itoa(i));
        uart_putc('\n');
        schedule();
    }
    uart_puts("FIN --> ");
    while(1){schedule();}
}

void suma10(){
    j += 10;
    schedule();
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{   
    (void) r0;
    (void) r1;
    (void) atags;
    
    uart_init();
    mem_init(((atag_t*)atags));
    uart_puts("\nMemoria inicializada\n");
    process_init();
    uart_puts("\nProcesos inicializados\n");
    
    /*
    create_kernel_thread(suma10, "proc1", 5);
    //print_processes();

    schedule();
    

    uart_puts("Valor de j --> ");
    uart_puts(itoa(j));
    uart_putc('\n');
    */

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
    //print_data();
    uart_puts("FIN --> main");
    
    while(1){schedule();}
}

