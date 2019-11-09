#ifndef	_P_MINI_UART_H
#define	_P_MINI_UART_H

#include <io/peripherals/base.h>
//all define in page 8 of BCM2837 and forward.

//Auxiliary interrupt status
#define AUX_IRQ (PBASE+0x00215000)
//Auxiliary functionality enable
#define AUX_ENABLES (PBASE+0x00215004) 
//mini uart i/o data
#define AUX_MU_IO (PBASE+0x00215040) 
//mini uart interrupt enable
#define AUX_MU_IER (PBASE+0x00215044) 
//mini uart interrupt identify
#define AUX_MU_IIR (PBASE+0x00215048) 
//mini uart line control
#define AUX_MU_LCR (PBASE+0x0021504C) 
//mini uart modem control
#define AUX_MU_MCR (PBASE+0x00215050) 
//mini uart line status
#define AUX_MU_LSR (PBASE+0x00215054) 
//mini uart modem status
#define AUX_MU_MSR (PBASE+0x00215058) 
//mini uart scratch
#define AUX_MU_SCRATCH (PBASE+0x0021505C) 
//mini uart control register
#define AUX_MU_CNTL (PBASE+0x00215060) 
//mini uart extra status
#define AUX_MU_STAT (PBASE+0x00215064) 
//mini uart baudrate register
#define AUX_MU_BAUD (PBASE+0x00215068) 
// SPI 1 Control register 0
#define AUX_SPI1_CNTL0 (PBASE+0x00215080)
// SPI 1 Control register 1
#define AUX_SPI1_CNTL1 (PBASE+0x00215084)
//SPI 1 status
#define AUX_SPI1_STAT (PBASE+0x00215088)
//SPI 1 data 
#define AUX_SPI1_IO (PBASE+0x00215090)
//SPI 1 Peek
#define AUX_SPI1_PEEK (PBASE+0x00215094)
//SPI 2 Control register 0   
#define AUX_SPI2_CNTL0 (PBASE+0x002150C0)
//SPI 2 Control register 
#define AUX_SPI2_CNTL1 (PBASE+0x002150C4)
//SPI 2 status
#define AUX_SPI2_STAT (PBASE+0x002150C8)
//SPI 2 data
#define AUX_SPI2_IO (PBASE+0x002150D0)
//SPI 2 Peek
#define AUX_SPI2_PEEK (PBASE+0x002150D4)

#endif  /*_P_MINI_UART_H */
