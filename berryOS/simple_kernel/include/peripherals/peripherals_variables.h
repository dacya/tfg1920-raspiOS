#ifndef _P_PERIPHERALS_VARIABLES_H
#define _P_PERIPHERALS_VARIABLES_H

#include <stdint.h>
#include "./gpio.h"
#include "./mini_uart.h"
#include "./uart.h"

//PL011 UART
uint32_t volatile* const UART0_FR_REG;
uint32_t volatile* const UART0_DR_REG;
uint32_t volatile* const UART0_CR_REG;
uint32_t volatile* const UART0_IBRD_REG;
uint32_t volatile* const UART0_FBRD_REG;
uint32_t volatile* const UART0_LCRH_REG;
uint32_t volatile* const UART0_ICR_REG;

//GPIO
uint32_t volatile* const GPFSEL0_REG;
uint32_t volatile* const GPFSEL1_REG;
uint32_t volatile* const GPPUD_REG;
uint32_t volatile* const GPPUDCLK0_REG;
uint32_t volatile* const GPPUDCLK1_REG;

//MINI_UART
uint32_t volatile* const AUX_MU_LSR_REG;
uint32_t volatile* const AUX_MU_IO_REG;
uint32_t volatile* const AUX_MU_MCR_REG;
uint32_t volatile* const AUX_MU_IER_REG;
uint32_t volatile* const AUX_MU_CNTL_REG;
uint32_t volatile* const AUX_ENABLES_REG;
uint32_t volatile* const AUX_MU_BAUD_REG;
uint32_t volatile* const AUX_MU_LCR_REG ;


#endif