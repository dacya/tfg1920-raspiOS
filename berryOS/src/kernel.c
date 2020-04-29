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

COMMAND test;

uint32_t critical_value = 0;
//1 locked 0 unlocked
int mutex = MUTEX_UNLOCK;

void test1_subroutine(void){
    int i = 0;
    uart_putln("Entered into the subroutine");
    uart_putln("Entered into the subroutine");
    uart_putln("Entered into the subroutine");
    uart_putln("Entered into the subroutine");
    uart_putln("test1: trying to take lock");
    take_lock(&mutex);
    while(1){
        if(i == 100000000){
            uart_putln("I'm test1 -->");
            i = 0;
        }
        i++;
    }
}

void test1(void){
    uint32_t j = 0;
    uint32_t i = 0;
    uart_putln("Executing test1");
    uart_putln("Executing test2");
    uart_putln("Executing test3");
    uart_putln("Executing test4");
    uart_putln("Executing test5");
    test1_subroutine();
}

void test2(void){
    uint32_t j = 0;
    uint32_t i = 0;
    uart_putln("test2: Trying to take lock");
    take_lock(&mutex);
    uart_putln("test2: lock taked by test2");

    while (1) {
        if(i == 1000000){
            uart_putln("I'm test2 -->");
            i = 0;
        }
        i++;    
    }
}

void test_command_handler(int argc, char** argv){
    
}

extern void pointer_test(void);

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {   
    (void) r0;
    (void) r1;
    (void) atags;
    pin_set_function(17, OUTPUT);
    
    test.helpText = "This is a test command";
    test.key = "test";
    test.trigger = test_command_handler;

    regcomm(&test_command_handler);

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
    #ifdef QUANTUM
    #define QUANTUM 100
    #endif
    local_timer_init(VIRTUAL_SYS, QUANTUM);
    enrichedPrintLn("[OK]", &GREEN, NULL);

    //TEST PROCESS SECTION
    //create_kernel_thread(&pointer_test, "nested call test", 16);
    create_kernel_thread(&test1, "Proc1", 5);
    create_kernel_thread(&test2, "Kezo", 5);

    print_processes();

}