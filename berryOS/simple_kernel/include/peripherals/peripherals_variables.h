#ifndef _P_PERIPHERALS_VARIABLES_H
#define _P_PERIPHERALS_VARIABLES_H

#include <stdint.h>
#include "peripherals/gpio.h"
#include "peripherals/mini_uart.h"
#include "peripherals/uart.h"

//                      PL011 UART
uint32_t volatile* const UART0_FR_REG;
uint32_t volatile* const UART0_DR_REG;
uint32_t volatile* const UART0_CR_REG;
uint32_t volatile* const UART0_IBRD_REG;
uint32_t volatile* const UART0_FBRD_REG;
uint32_t volatile* const UART0_LCRH_REG;
uint32_t volatile* const UART0_ICR_REG;
uint32_t volatile* const UART0_IMSC_REG;
//----------------------FIN PL011 UART-----------------------------


//                      GPIO
uint32_t volatile* const GPFSEL0_REG;
uint32_t volatile* const GPFSEL1_REG;
uint32_t volatile* const GPFSEL2_REG;
uint32_t volatile* const GPFSEL3_REG;
uint32_t volatile* const GPFSEL4_REG;
uint32_t volatile* const GPFSEL5_REG;

uint32_t volatile* const GPSET0_REG;
uint32_t volatile* const GPSET1_REG;

uint32_t volatile* const GPCLR0_REG;
uint32_t volatile* const GPCLR1_REG;

uint32_t volatile* const GPLEV0_REG;
uint32_t volatile* const GPLEV1_REG;

uint32_t volatile* const GPEDS0_REG;
uint32_t volatile* const GPEDS1_REG;

uint32_t volatile* const GPREN0_REG;
uint32_t volatile* const GPREN1_REG;

uint32_t volatile* const GPFEN0_REG;
uint32_t volatile* const GPFEN1_REG;

uint32_t volatile* const GPHEN0_REG;
uint32_t volatile* const GPHEN1_REG;

uint32_t volatile* const GPLEN0_REG;
uint32_t volatile* const GPLEN1_REG;

uint32_t volatile* const GPAREN0_REG;
uint32_t volatile* const GPAREN1_REG;

uint32_t volatile* const GPALEN0_REG;
uint32_t volatile* const GPALEN1_REG;

uint32_t volatile* const GPPUD_REG;
uint32_t volatile* const GPPUDCLK0_REG;
uint32_t volatile* const GPPUDCLK1_REG;
//----------------------FIN GPIO-----------------------------------


//                      MINI_UART
uint32_t volatile* const AUX_MU_LSR_REG;
uint32_t volatile* const AUX_MU_IO_REG;
uint32_t volatile* const AUX_MU_MCR_REG;
uint32_t volatile* const AUX_MU_IER_REG;
uint32_t volatile* const AUX_MU_CNTL_REG;
uint32_t volatile* const AUX_ENABLES_REG;
uint32_t volatile* const AUX_MU_BAUD_REG;
uint32_t volatile* const AUX_MU_LCR_REG;
//----------------------FIN MINI_UART-----------------------------------

#endif