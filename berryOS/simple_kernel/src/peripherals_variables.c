#include "../include/peripherals/peripherals_variables.h"

//PL011 UART
uint32_t volatile* const UART0_FR_REG = (uint32_t*)UART0_FR;
uint32_t volatile* const UART0_DR_REG = (uint32_t*)UART0_DR;
uint32_t volatile* const UART0_CR_REG = (uint32_t*)UART0_CR;
uint32_t volatile* const UART0_IBRD_REG = (uint32_t*)UART0_IBRD;
uint32_t volatile* const UART0_FBRD_REG = (uint32_t*)UART0_FBRD;
uint32_t volatile* const UART0_LCRH_REG = (uint32_t*)UART0_LCRH;
uint32_t volatile* const UART0_ICR_REG = (uint32_t*)UART0_ICR;

//GPIO
uint32_t volatile* const GPFSEL0_REG= (uint32_t*)GPFSEL0;
uint32_t volatile* const GPFSEL1_REG= (uint32_t*)GPFSEL1;
uint32_t volatile* const GPPUD_REG = (uint32_t*)GPPUD;
uint32_t volatile* const GPPUDCLK0_REG = (uint32_t*)GPPUDCLK0;
uint32_t volatile* const GPPUDCLK1_REG = (uint32_t*)GPPUDCLK1;

//MINI_UART
uint32_t volatile* const AUX_MU_LSR_REG = (uint32_t*)AUX_MU_LSR;
uint32_t volatile* const AUX_MU_IO_REG = (uint32_t*)AUX_MU_IO;
uint32_t volatile* const AUX_MU_MCR_REG = (uint32_t*)AUX_MU_MCR;
uint32_t volatile* const AUX_MU_IER_REG = (uint32_t*)AUX_MU_IER;
uint32_t volatile* const AUX_MU_CNTL_REG = (uint32_t*)AUX_MU_CNTL;
uint32_t volatile* const AUX_ENABLES_REG = (uint32_t*)AUX_ENABLES;
uint32_t volatile* const AUX_MU_BAUD_REG = (uint32_t*)AUX_MU_BAUD;
uint32_t volatile* const AUX_MU_LCR_REG = (uint32_t*)AUX_MU_LCR;

