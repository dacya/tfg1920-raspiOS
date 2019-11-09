#ifndef	_P_MINI_UART_H
#define	_P_MINI_UART_H

#include "peripherals/base.h"
//all define in page 8 of BCM2837 and forward.

//Auxiliary functionality enable
#define AUX_ENABLES     (PBASE+0x00215004) 
//mini uart i/o data
#define AUX_MU_IO   (PBASE+0x00215040) 
//mini uart interrupt enable
#define AUX_MU_IER  (PBASE+0x00215044) 
//mini uart interrupt identify
#define AUX_MU_IIR  (PBASE+0x00215048) 
//mini uart line control
#define AUX_MU_LCR  (PBASE+0x0021504C) 
//mini uart modem control
#define AUX_MU_MCR  (PBASE+0x00215050) 
//mini uart line status
#define AUX_MU_LSR  (PBASE+0x00215054) 
//mini uart modem status
#define AUX_MU_MSR  (PBASE+0x00215058) 
//mini uart scratch
#define AUX_MU_SCRATCH  (PBASE+0x0021505C) 
//mini uart control register
#define AUX_MU_CNTL (PBASE+0x00215060) 
//mini uart extra status
#define AUX_MU_STAT (PBASE+0x00215064) 
//mini uart baudrate register
#define AUX_MU_BAUD (PBASE+0x00215068) 

#endif  /*_P_MINI_UART_H */
