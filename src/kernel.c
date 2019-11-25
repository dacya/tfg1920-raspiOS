#include <stddef.h>
#include <stdint.h>
#include <io/peripherals/uart.h>
#include <io/peripherals/gpio.h>

// Loop <delay> times in a way that the compiler won't optimize away
static inline void delay(int32_t count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
            : "=r"(count): [count]"0"(count) : "cc");
}

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
    while ( *(UART0_FR) & (1 << 5) ) { }
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

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    // Declare as unused
    (void) r0;
    (void) r1;
    (void) atags;

    uart_init();
    uart_puts("Hello, kernel World!\r\n");
    char a;

    while (1) {
        uart_putc('>');
        while ((a = uart_getc()) != '\r' ){
            uart_putc(a);
        }
        uart_putc('\n');
    }
}
