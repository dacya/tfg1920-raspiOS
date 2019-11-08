#ifndef	_P_MINI_UART_H
#define	_P_MINI_UART_H

#include "peripherals/base.h"
//all define in page 8 of BCM2837 and forward.

//Auxiliary functionality enable
uint32_t volatile * const AUX_ENABLES = (uint32_t *)(PBASE+0x00215004); 
//mini uart i/o data
uint32_t volatile * const AUX_MU_IO_REG = (uint32_t *)(PBASE+0x00215040); 
//mini uart interrupt enable
uint32_t volatile * const AUX_MU_IER_REG = (uint32_t *)(PBASE+0x00215044); 
//mini uart interrupt identify
uint32_t volatile * const AUX_MU_IIR_REG = (uint32_t *)(PBASE+0x00215048); 
//mini uart line control
uint32_t volatile * const AUX_MU_LCR_REG = (uint32_t *)(PBASE+0x0021504C); 
//mini uart modem control
uint32_t volatile * const AUX_MU_MCR_REG = (uint32_t *)(PBASE+0x00215050); 
//mini uart line status
uint32_t volatile * const AUX_MU_LSR_REG = (uint32_t *)(PBASE+0x00215054); 
//mini uart modem status
uint32_t volatile * const AUX_MU_MSR_REG = (uint32_t *)(PBASE+0x00215058); 
//mini uart scratch
uint32_t volatile * const AUX_MU_SCRATCH = (uint32_t *)(PBASE+0x0021505C); 
//mini uart control register
uint32_t volatile * const AUX_MU_CNTL_REG = (uint32_t *)(PBASE+0x00215060); 
//mini uart extra status
uint32_t volatile * const AUX_MU_STAT_REG = (uint32_t *)(PBASE+0x00215064); 
//mini uart baudrate register
uint32_t volatile * const AUX_MU_BAUD_REG = (uint32_t *)(PBASE+0x00215068); 

#endif  /*_P_MINI_UART_H */
