#include <stddef.h>
#include <stdint.h>
#include <interrupts.h>
#include <local_timer.h>
#include <io/stdio.h>
#include <io/uart.h>
#include <io/gpu.h>
#include <io/gpio.h>
#include <proc/pcb.h>
#include <console/console.h>
#include <console/command.h>
#include <proc/locks/mutex.h>

extern void io_halt();

COMMAND lawebada;

uint32_t critical_value = 0;
//1 locked 0 unlocked
int mutex = MUTEX_UNLOCK;

void test1(void){
    uint32_t j = 0;
    uint32_t i = 0;
    uart_putln("Executing test1");
    take_lock(&mutex);
    uart_putln("mutex took");
    while (1) {
        if(i == 1000000000){
            uart_putln("I'm test1 -->");
            i = 0;
        }
        i++;    
    }
}

void test2(void){
    uint32_t j = 0;
    uint32_t i = 0;
    uart_putln("Executing test2");
    take_lock(&mutex);
    uart_putln("lock taked by test2");

    while (1) {
        if(i == 1000000000){
            uart_putln("I'm test2 -->");
            i = 0;
        }
        i++;    
    }
}

void lawebada_handler(int argc, char** argv){
    printLn("This command works");
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {   
    (void) r0;
    (void) r1;
    (void) atags;
    pin_set_function(17, OUTPUT);
    
    lawebada.helpText = "I'm goin to end this whole man's career";
    lawebada.key = "lawebada";
    lawebada.trigger = lawebada_handler;

    regcomm(&lawebada);

    /* UART */
    uart_init();
    uart_puts(">> Uart init");
    uart_putln("[OK]");

    /* DYNAMIC MEMORY */
    uart_puts(">> (uart)Dynamic memory: ");
    mem_init(((atag_t *)atags));
    uart_putln(" [OK]");

    /* HDMI */
    gpu_init();
    print(">> Dynamic memory: ");
    enrichedPrintLn("[OK]", &GREEN, NULL);
    print(">> GPU init: ");
    enrichedPrintLn("[OK]", &GREEN, NULL);

    /* Processes */
    print(">> Processes init: ");
    process_init();
    enrichedPrintLn("[OK]", &GREEN, NULL);

    /* Commands */
    print("\t- Commands init: ");
    init_commands();
    enrichedPrintLn("[OK]", &GREEN, NULL);
    
    /* Console */
    print(">> Console init: ");
    start_console();
    enrichedPrintLn("[OK]", &GREEN, NULL);

    /* INTERRUPTS */
    print(">> Interrupts init: ");
    interrupts_init();
    enrichedPrintLn("[OK]", &GREEN, NULL);

    /* LOCAL TIMER */
    print("\t- Register timer handler and clearer: ");
    register_irq_handler(ARM_TIMER, local_timer_handler, local_timer_clearer);
    enrichedPrintLn("[OK]", &GREEN, NULL);

    print(">> Local timer init: ");
    local_timer_init(VIRTUAL_SYS, 5000);
    enrichedPrintLn("[OK]", &GREEN, NULL);

    //TEST PROCESS SECTION
    create_kernel_thread(&test1, "Proc1", 5);
    create_kernel_thread(&test2, "Kezo", 5);

    print_processes();

}