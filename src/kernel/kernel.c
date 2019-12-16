/**
 * 
 * Raúl Sánchez Montaño
 * 
 * Dic 16, 2019
 */

#include <stddef.h>
#include <stdint.h>
#include <kernel/uart.h>
#include <kernel/atag.h>
#include <kernel/mem.h>
#include <common/stdio.h>
#include <common/stdlib.h>

volatile uint32_t* gpio_fs1 = (uint32_t*)(GPIO_BASE+0x04);

volatile uint32_t* gpio_set0 = (uint32_t*)(GPIO_BASE+0x1C);

volatile uint32_t* gpio_clr0 = (uint32_t*)(GPIO_BASE+0x28);


void delay(int32_t count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
            : "=r"(count): [count]"0"(count) : "cc");
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags){

    (void) r0;
    (void) r1;
    (void) atags;

    uart_init();
    mem_init(((atag_t *)atags));

    // *gpio_fs1 |= (uint32_t)(1<<21);
    // int32_t tim = 1000000;
    // *gpio_set0 |= (uint32_t)(1<<17);
    // *gpio_clr0 |= (uint32_t)(1<<17);


    delay(1000);
    uart_puts("Bienvenido a BerryOS\n");
    delay(1000);
    
    int first = 1;
    void * firstt;
    while (1) {
        void * dest;
        char c;
        unsigned int i = 0;
        unsigned char buf[100];
        uart_puts("¿Que quieres hacer?\n > 1 --> Guardar cadena de texto\n > 2 --> Ver cadenas\n > 3 --> Liberar última cadena\n > 4 --> Liberar primera cadena\n > ");
        
        c = uart_getc();
        uart_putc(c);
        uart_putc('\n');
        if(c == '1'){
            
            uart_puts("Inserte cadena > ");
             
            
            while((c = uart_getc()) != '\r')  {
                uart_putc(c);
                buf[i++] = c;
            }
            uart_putc('\n');
            uart_puts("     Cadena insertada -->");
            uart_puts(buf);
            uart_putc('\n');

            buf[i++] = '\0';            
            dest = kmalloc(i * sizeof(char));
            if(first){
                first = 0;
                firstt = dest;
            }
            memcpy(dest, buf, i * sizeof(char));
            
        }
        else if (c == '2'){
            print_data();
        }
        else if (c == '3'){
            kfree(dest);
        }
        else if (c == '4'){
            kfree(firstt);
        }
    }
/*
    uart_puts("Hello, kernel World!\r\n");

    while (1) {
        uart_putc(uart_getc());
        uart_putc('\n');
    }
*/
}