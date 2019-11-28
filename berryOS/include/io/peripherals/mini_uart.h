#ifndef	_P_MINI_UART_H
#define	_P_MINI_UART_H

#include <io/peripherals/base.h>
#include <stdint.h>

//all define in page 8 of BCM2837 and forward.
#define MINI_UART_BASE (PHYSICAL_PBASE + 0x00215000)

#define AUX_IRQ        ((volatile uint32_t *)(MINI_UART_BASE+0x000)) // Auxiliary interrupt status
#define AUX_ENABLES    ((volatile uint32_t *)(MINI_UART_BASE+0x004)) // Auxiliary functionality enable
#define AUX_MU_IO      ((volatile uint32_t *)(MINI_UART_BASE+0x040)) // mini uart i/o data
#define AUX_MU_IER     ((volatile uint32_t *)(MINI_UART_BASE+0x044)) // mini uart interrupt enable
#define AUX_MU_IIR     ((volatile uint32_t *)(MINI_UART_BASE+0x048)) // mini uart interrupt identify
#define AUX_MU_LCR     ((volatile uint32_t *)(MINI_UART_BASE+0x04C)) // mini uart line control
#define AUX_MU_MCR     ((volatile uint32_t *)(MINI_UART_BASE+0x050)) // mini uart modem control
#define AUX_MU_LSR     ((volatile uint32_t *)(MINI_UART_BASE+0x054)) // mini uart line status
#define AUX_MU_MSR     ((volatile uint32_t *)(MINI_UART_BASE+0x058)) // mini uart modem status
#define AUX_MU_SCRATCH ((volatile uint32_t *)(MINI_UART_BASE+0x05C)) // mini uart scratch
#define AUX_MU_CNTL    ((volatile uint32_t *)(MINI_UART_BASE+0x060)) // mini uart control register
#define AUX_MU_STAT    ((volatile uint32_t *)(MINI_UART_BASE+0x064)) // mini uart extra status
#define AUX_MU_BAUD    ((volatile uint32_t *)(MINI_UART_BASE+0x068)) // mini uart baudrate register
#define AUX_SPI1_CNTL0 ((volatile uint32_t *)(MINI_UART_BASE+0x080)) // SPI 1 Control register 0
#define AUX_SPI1_CNTL1 ((volatile uint32_t *)(MINI_UART_BASE+0x084)) // SPI 1 Control register 1
#define AUX_SPI1_STAT  ((volatile uint32_t *)(MINI_UART_BASE+0x088)) // SPI 1 status
#define AUX_SPI1_IO    ((volatile uint32_t *)(MINI_UART_BASE+0x090)) // SPI 1 data 
#define AUX_SPI1_PEEK  ((volatile uint32_t *)(MINI_UART_BASE+0x094)) // SPI 1 Peek
#define AUX_SPI2_CNTL0 ((volatile uint32_t *)(MINI_UART_BASE+0x0C0)) // SPI 2 Control register 0   
#define AUX_SPI2_CNTL1 ((volatile uint32_t *)(MINI_UART_BASE+0x0C4)) // SPI 2 Control register 
#define AUX_SPI2_STAT  ((volatile uint32_t *)(MINI_UART_BASE+0x0C8)) // SPI 2 status
#define AUX_SPI2_IO    ((volatile uint32_t *)(MINI_UART_BASE+0x0D0)) // SPI 2 data
#define AUX_SPI2_PEEK  ((volatile uint32_t *)(MINI_UART_BASE+0x0D4)) // SPI 2 Peek

void mini_uart_init ( void );
uint8_t mini_uart_recv ( void );
void mini_uart_send ( uint8_t c );
void mini_uart_send_string(char* str);
void mini_uart_transmit_reg( void );

#endif  /*_P_MINI_UART_H */
