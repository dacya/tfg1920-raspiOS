#include <stddef.h>
#include <stdint.h>

#include <io/uart.h>
#include <interrupts.h>
#include <local_timer.h>

extern void io_halt();

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    // Declare as unused
    (void) r0;
    (void) r1;
    (void) atags;

    uart_init();
    
    asm volatile("mov %0, lr": "=r" (r0):);
    uart_puts("r0 value is: ");
    uart_hex_puts(r0);

    local_timer_init();
    interrupts_init();
    //register_irq_handler(ARM_TIMER, local_timer_handler, local_timer_clearer);

    uart_puts("Hello, World!\r\n");
    
    //just for testing asm function
    uart_puts("CPSR.MODE[4:0] value is: ");
    uint32_t salida;
    asm volatile("mrs r0, cpsr");
    asm volatile("and r0, r0, #0x1f");
    asm volatile("mov %0, r0": "=r"(salida));
    uart_hex_puts(salida);


    uart_puts("sp value is: ");
    asm volatile("mov %0, sp": "=r"(salida):);
    uart_hex_puts(salida);
    uart_puts("lr value is: ");
    asm volatile("mov %0, lr": "=r"(salida):);
    uart_hex_puts(salida);
    while (1) {}
}
