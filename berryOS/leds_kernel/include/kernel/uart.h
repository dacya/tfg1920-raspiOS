#include <stddef.h>
#include <stdint.h>

#ifndef UART_H
#define UART_H

static inline void mmio_write(uint32_t reg, uint32_t data);

static inline uint32_t mmio_read(uint32_t reg);

static inline void delay(int32_t count);

void uart_init();

void uart_putc(unsigned char c);

unsigned char uart_getc();

void uart_puts(const char* str);

//(AlexCCo) Instead of an enum it's preferably using the directive #define 
enum
{
    // The GPIO registers base address.
    GPIO_BASE = 0x3F200000UL, // for raspi2 & 3, 0x20200000 for raspi1

    /* Notes done by AlexCCo:
          Basing of the BroadCom documentation, GPIO_BASE should store the value
          0x3F000000 to access at the peripherals directly (0x7E000000 in other
          case)
          This is a comment to clarify the real GPIO_BASE value it should be
          used.
    */

    GPPUD = (GPIO_BASE + 0x94),
    //(AlexCCo)GPPUD at 0x3F200094
    GPPUDCLK0 = (GPIO_BASE + 0x98),

    // The base address for UART.
    UART0_BASE = 0x3F201000, // for raspi2 & 3, 0x20201000 for raspi1
    //(AlexCCo) 0x7E201000 for virtual memory

    /*
    (AlexCCo) all of these registers are documentend on section 13.4 register
    view from UART section in BCM2837, BCM2836 & BCM2835  
    */
    UART0_DR     = (UART0_BASE + 0x00), //UART data register
    UART0_RSRECR = (UART0_BASE + 0x04), /*UART receive status register/error 
                                          clear register */
    UART0_FR     = (UART0_BASE + 0x18), //UART flag register
    UART0_ILPR   = (UART0_BASE + 0x20), //it does nothing, do not use
    UART0_IBRD   = (UART0_BASE + 0x24), //UART integer baudrate data
    UART0_FBRD   = (UART0_BASE + 0x28), //UART fractional baudrate data
    UART0_LCRH   = (UART0_BASE + 0x2C), //UART line control register
    UART0_CR     = (UART0_BASE + 0x30), //UART control register
    UART0_IFLS   = (UART0_BASE + 0x34), //UART interrupt fifo level select
    UART0_IMSC   = (UART0_BASE + 0x38), //UART interrupt mask set/clear
    UART0_RIS    = (UART0_BASE + 0x3C), //UART raw interrupt status register
    UART0_MIS    = (UART0_BASE + 0x40), //UART masked interrupt status register
    UART0_ICR    = (UART0_BASE + 0x44), //UART interrupt clear register
    UART0_DMACR  = (UART0_BASE + 0x48), //it does nothing, do not use
    UART0_ITCR   = (UART0_BASE + 0x80), //UART test control ITCR register
    UART0_ITIP   = (UART0_BASE + 0x84), //UART test control ITIP register
    UART0_ITOP   = (UART0_BASE + 0x88), //UART test control ITOP register
    UART0_TDR    = (UART0_BASE + 0x8C), //UART test data register
    /* 
        The UART0_IBRD, UART0_FBRD & UART0_LCRH must not be changed when:
          -The UART is enabled
          -The completing a transmission or a reception when it has been programmed
           to become disabled. 
        
    */ 
};


#endif
