#ifndef _P_UART_H
#define _P_UART_H

#include "peripherals/base.h"
//Everything is documented in section 13 of BCM2837, BCM2836 & BCM2835


/*
  PL011 Uart data register
  ------------------------
  bit 31-12: Reserved
  bit 11: Overrun Error
  bit 10: Break error
  bit 9: Parity error
  bit 8: Framing error
  bit 7-0: Data
*/
#define UART0_DR (PBASE+0x00201000)
/*
  PL011 Uart receive status register/error clear register
  ------------------------
  bit 31-4: Reserved
  bit 3: Overrun error
  bit 2: Break error
  bit 1: Parity error
  bit 0: Framing error
*/
#define UART0_RSRECR (PBASE+0x00201004)
/*
  PL011 Uart flag register
  ------------------------
  bit 31-9: Reserved
  bit 8: Unsupported
  bit 7: TX FIFO empty
  bit 6: RX FIFO empty
  bit 5: TX FIFO full
  bit 4: RX FIFO full
  bit 3: UART busy
  bit 2: Unsupported
  bit 1: Unsupported
  bit 0: Clear To Send (complement of nUARTCTS modem statu)
*/
#define UART0_FR (PBASE+0x00201018)
//DISABLE NOT USE
#define UART0_ILPR (PBASE+0x00201020)
/*
  PL011 Uart integer baud rate divisor
  ------------------------
  bit 31-16: Reserved
  bit 15-0: The integer Baud rate divisor
*/
#define UART0_IBRD (PBASE+0x00201024)
/*
  PL011 Uart fractional baud rate divisor
  ------------------------
  bit 31-6: Reserved
  bit 5-0: The fractional Baud rate divisor
*/
#define UART0_FBRD (PBASE+0x00201028)
/*
  PL011 Uart line control register
  ------------------------
  This register must not be change when:
    -UART is enable
    -Completing a transmission or reception when it has been programmed to
     become disabled

  bit 31-8: Reserved
  bit 7: Sticky parity select
  bit 6-5: Word length
  bit 4: Enable FIFO's
  bit 3: Two stop bit select send at the end of the frame
  bit 2: Even parity select
  bit 1: Parity enable
  bit 0: Send break
*/
#define UART0_LCRH (PBASE+0x0020102C)
/*
  PL011 Uart control register
  ------------------------
  bit 32-16: Reserved
  bit 15: Clear To Send hardware flow control enable
  bit 14: Request To Send hardware flow control enable
  bit 13: Doesn't matter
  bit 12: Doesn't matter
  bit 11: Request To Send (complement of nUARTRTS modem statu)
  bit 10: Doesn't matter
  bit 9: Receive enable 
  bit 8: Transmit enable
  bit 7: Loopback enable
  bit 6-3: Reserved
  bit 2: Doesn't matter
  bit 1: Doesn't matter
  bit 0: UART enable 
*/
#define UART0_CR (PBASE+0x00201030)
/*
  PL011 Uart interrupt FIFO level select register
  ------------------------
*/
#define UART0_IFLS (PBASE+0x00201034)
/*
  PL011 Uart interrupt mask set clear register
  ------------------------
  bit 31-11: Reserved
  bit 10: Overrun Error
  bit 9: Break error
  bit 8: Parity error
  bit 7: Framing error
  bit 6: Receive timeout error
  bit 5: Transmit interrupt
  bit 4: Receive interrupt
  bit 3: Doesn't matter
  bit 2: Doesn't matter
  bit 1: nUARTCTS modem interrupt
  bit 0: Doesn't matter
*/
#define UART0_IMSC (PBASE+0x00201038)
/*
  PL011 Uart raw interrupt status register
  ------------------------
*/
#define UART0_RIS (PBASE+0x0020103C)
/*
  PL011 Uart masked interupt status register
  ------------------------
*/
#define UART0_MIS (PBASE+0x00201040)
/*
  PL011 Uart interrupt clear register
  ------------------------
  bit 31-11: Reserved
  bit 10: Overrun error interrupt clear (clear the UARTOEINTR)
  bit 9: Break interrupt clear (clear UARTBEINTR)
  bit 8: Parity error interrupt clear (clear UARTPEINTR)
  bit 7: Framing error interrupt clear (clear UARTFEINTR)
  bit 6: Receive timeout interrupt clear (clear UARTRTINTR)
  bit 5: Transmit interrupt clear (clear UARTTXINTR)
  bit 4: Receive interrupt clear (clear UARTRXINTR)
  bit 3: unsupported, always zero
  bit 2: unsupported, always zero
  bit 1: (Clear To Send) nUARTCTS modem mask interrupt status from UARTCTSINTR
  bit 0: unsupported, always zero
  NOTE: putting an 1 to clear and a 0 to do nothing.
*/
#define UART0_ICR (PBASE+0x00201044)
/*
  L011 Uart DMA control register
  ------------------------
*/
#define UART0_DMACR (PBASE+0x00201048)
/* 
  PL011 Uart text control register
  ------------------------
*/
#define UART0_ITCR (PBASE+0x00201080)
/*
  PL011 Uart integration test input register
  ------------------------
*/
#define UART0_ITIP (PBASE+0x00201084)
/*
  PL011 Uart integration test output register
  ------------------------
*/
#define UART0_ITOP (PBASE+0x00201088)
/*
  PL011 Uart test data register
  ------------------------
*/
#define UART0_TDR (PBASE+0x0020108C)

#endif