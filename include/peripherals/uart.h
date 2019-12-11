/**
 * The purpose of this library is to create a friendly approach to use
 * the PL011 UART0 incorporated inside Raspberry Pi 2.
 * 
 * Alejandro Cancelo Correia
 * 
 * Nov, 26, 2019
*/

#ifndef _PL011_UART_H
#define _PL011_UART_H

#include <stdint.h>
#include <peripherals/base.h>
/*
// The GPIO registers base address.
#define GPIO_BASE 0x3F200000 // for raspi2 & 3, 0x20200000 for raspi1

// The offsets for reach register.
// Controls actuation of pull up/down to ALL GPIO pins.
#define GPPUD ((volatile uint32_t *) (GPIO_BASE + 0x94))

// Controls actuation of pull up/down for specific GPIO pin.
#define GPPUDCLK0 ((volatile uint32_t *) (GPIO_BASE + 0x98))
*/

// The base address for UART.
#define UART0_BASE (PHYSICAL_PBASE + 0x00201000) // for raspi2 & 3, 0x20201000 for raspi1

// The offsets for reach register for the UART.
#define UART0_DR     ((volatile uint32_t *) (UART0_BASE + 0x00))
#define UART0_RSRECR ((volatile uint32_t *) (UART0_BASE + 0x04))
#define UART0_FR     ((volatile uint32_t *) (UART0_BASE + 0x18))
#define UART0_ILPR   ((volatile uint32_t *) (UART0_BASE + 0x20))
#define UART0_IBRD   ((volatile uint32_t *) (UART0_BASE + 0x24))
#define UART0_FBRD   ((volatile uint32_t *) (UART0_BASE + 0x28))
#define UART0_LCRH   ((volatile uint32_t *) (UART0_BASE + 0x2C))
#define UART0_CR     ((volatile uint32_t *) (UART0_BASE + 0x30))
#define UART0_IFLS   ((volatile uint32_t *) (UART0_BASE + 0x34))
#define UART0_IMSC   ((volatile uint32_t *) (UART0_BASE + 0x38))
#define UART0_RIS    ((volatile uint32_t *) (UART0_BASE + 0x3C))
#define UART0_MIS    ((volatile uint32_t *) (UART0_BASE + 0x40))
#define UART0_ICR    ((volatile uint32_t *) (UART0_BASE + 0x44))
#define UART0_DMACR  ((volatile uint32_t *) (UART0_BASE + 0x48))
#define UART0_ITCR   ((volatile uint32_t *) (UART0_BASE + 0x80))
#define UART0_ITIP   ((volatile uint32_t *) (UART0_BASE + 0x84))
#define UART0_ITOP   ((volatile uint32_t *) (UART0_BASE + 0x88))
#define UART0_TDR    ((volatile uint32_t *) (UART0_BASE + 0x8C))

void uart_init();
void uart_putc(unsigned char c);
unsigned char uart_getc();
void uart_puts(const char* str);
void clean_buf(char *buf, int size);

// Loop <delay> times in a way that the compiler won't optimize away
static inline void delay(int32_t count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
            : "=r"(count): [count]"0"(count) : "cc");
}

#endif