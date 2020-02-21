/**
 * 
 * Alejandro Cancelo Correia
 * 
 * Nov, 26, 2019
*/

#include <stddef.h>
#include <peripherals/uart.h>
#include <peripherals/gpio.h>


void uart_init()
{
    // Disable UART0.
    *(UART0_CR) = 0x00000000;
    // Setup the GPIO pin 14 && 15.

    // Disable pull up/down for all GPIO pins & delay for 150 cycles.
    *(GPPUD) = 0x00000000;
    delay(150);

    // Disable pull up/down for pin 14,15 & delay for 150 cycles.
    *(GPPUDCLK0) = (1 << 14) | (1 << 15);
    delay(150);

    // Write 0 to GPPUDCLK0 to make it take effect.
    *(GPPUDCLK0) = 0x00000000;

    // Clear pending interrupts.
    *(UART0_ICR) = 0x7FF;

    // Set integer & fractional part of baud rate.
    // Divider = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // UART_CLOCK = 3000000; Baud = 115200.

    // Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
    *(UART0_IBRD) = 1;
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    *(UART0_FBRD) = 40;

    // Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
    *(UART0_LCRH) = (1 << 4) | (1 << 5) | (1 << 6);

    // Mask all interrupts.
    *(UART0_IMSC) = (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
            (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10);

    // Enable UART0, receive & transfer part of UART.
    *(UART0_CR) = (1 << 0) | (1 << 8) | (1 << 9);
}

void uart_putc(unsigned char c)
{
    // Wait for UART to become ready to transmit.
    while ( *(UART0_FR) & (1 << 5) );
    /* this doesn't work and i don't know why
    *(UART0_DR) &= (~0xFF);
    *(UART0_DR) |= (uint8_t)c;
    */

   *(UART0_DR) = c;
}

unsigned char uart_getc()
{
    // Wait for UART to have received something.
    while ( *(UART0_FR) & (1 << 4) ) { }
    return *(UART0_DR);
}

void uart_puts(const char* str)
{
    for (size_t i = 0; str[i] != '\0'; i ++)
        uart_putc((unsigned char)str[i]);
}

void convert_to_str(uint32_t value, char *buff, int size){
    char reminder;
    while(value != 0){
        reminder = value & 0xF;
        value = value >> 4;
        if(reminder < 10){
            buff[--size] = 48 + reminder;
        }
        else {
            buff[--size] = 55 + reminder;
        }
    }
}

void uart_hex_puts(uint32_t value)
{
    char str_argument[9] = {'0','0','0','0','0','0','0','0', '\0'};
    convert_to_str(value, str_argument, 8);
    uart_puts("0x");
    uart_puts(str_argument);
    uart_puts("\r\n");
}

void clean_buf(char *buf, int size)
{
    for(int i = 0; i < size; i++)
    {
        buf[i] = 0;
    }
}
