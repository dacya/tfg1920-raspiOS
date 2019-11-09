#include "../include/peripherals/peripherals_variables.h"

//                      PL011 UART
uint32_t volatile* const UART0_FR_REG = (uint32_t*)UART0_FR;
uint32_t volatile* const UART0_DR_REG = (uint32_t*)UART0_DR;
uint32_t volatile* const UART0_CR_REG = (uint32_t*)UART0_CR;
uint32_t volatile* const UART0_IBRD_REG = (uint32_t*)UART0_IBRD;
uint32_t volatile* const UART0_FBRD_REG = (uint32_t*)UART0_FBRD;
uint32_t volatile* const UART0_LCRH_REG = (uint32_t*)UART0_LCRH;
uint32_t volatile* const UART0_ICR_REG = (uint32_t*)UART0_ICR;
uint32_t volatile* const UART0_IMSC_REG = (uint32_t*)UART0_IMSC;
//----------------------FIN PL011 UART-----------------------------


//                      GPIO
uint32_t volatile* const GPFSEL0_REG= (uint32_t*)GPFSEL0;
uint32_t volatile* const GPFSEL1_REG= (uint32_t*)GPFSEL1;
uint32_t volatile* const GPFSEL2_REG= (uint32_t*)GPFSEL2;
uint32_t volatile* const GPFSEL3_REG= (uint32_t*)GPFSEL3;
uint32_t volatile* const GPFSEL4_REG= (uint32_t*)GPFSEL4;
uint32_t volatile* const GPFSEL5_REG= (uint32_t*)GPFSEL5;

uint32_t volatile* const GPSET0_REG = (uint32_t*)GPSET0;
uint32_t volatile* const GPSET1_REG = (uint32_t*)GPSET1;

uint32_t volatile* const GPCLR0_REG = (uint32_t*)GPCLR0;
uint32_t volatile* const GPCLR1_REG = (uint32_t*)GPCLR1;

uint32_t volatile* const GPLEV0_REG = (uint32_t*)GPLEV0;
uint32_t volatile* const GPLEV1_REG = (uint32_t*)GPLEV1;

uint32_t volatile* const GPEDS0_REG = (uint32_t*)GPEDS0;
uint32_t volatile* const GPEDS1_REG = (uint32_t*)GPEDS1;

uint32_t volatile* const GPREN0_REG = (uint32_t*)GPREN0;
uint32_t volatile* const GPREN1_REG = (uint32_t*)GPREN1;

uint32_t volatile* const GPFEN0_REG = (uint32_t*)GPFEN0;
uint32_t volatile* const GPFEN1_REG = (uint32_t*)GPFEN1;

uint32_t volatile* const GPHEN0_REG = (uint32_t*)GPHEN0;
uint32_t volatile* const GPHEN1_REG = (uint32_t*)GPHEN1;

uint32_t volatile* const GPLEN0_REG = (uint32_t*)GPLEN0;
uint32_t volatile* const GPLEN1_REG = (uint32_t*)GPLEN1;

uint32_t volatile* const GPAREN0_REG = (uint32_t*)GPAREN0;
uint32_t volatile* const GPAREN1_REG = (uint32_t*)GPAREN1;

uint32_t volatile* const GPALEN0_REG = (uint32_t*)GPALEN0;
uint32_t volatile* const GPALEN1_REG = (uint32_t*)GPALEN1;

uint32_t volatile* const GPPUD_REG = (uint32_t*)GPPUD;
uint32_t volatile* const GPPUDCLK0_REG = (uint32_t*)GPPUDCLK0;
uint32_t volatile* const GPPUDCLK1_REG = (uint32_t*)GPPUDCLK1;
//----------------------FIN GPIO-----------------------------------


//                      MINI_UART
uint32_t volatile* const AUX_ENABLES_REG = (uint32_t*)AUX_ENABLES;
uint32_t volatile* const AUX_MU_IO_REG = (uint32_t*)AUX_MU_IO;
uint32_t volatile* const AUX_MU_IER_REG = (uint32_t*)AUX_MU_IER;
uint32_t volatile* const AUX_MU_IIR_REG = (uint32_t*)AUX_MU_IIR;
uint32_t volatile* const AUX_MU_LCR_REG = (uint32_t*)AUX_MU_LCR;
uint32_t volatile* const AUX_MU_MCR_REG = (uint32_t*)AUX_MU_MCR;
uint32_t volatile* const AUX_MU_LSR_REG = (uint32_t*)AUX_MU_LSR;
uint32_t volatile* const AUX_MU_MSR_REG = (uint32_t*)AUX_MU_MSR;
uint32_t volatile* const AUX_MU_SCRATCH_REG = (uint32_t*)AUX_MU_SCRATCH;
uint32_t volatile* const AUX_MU_CNTL_REG = (uint32_t*)AUX_MU_CNTL;
uint32_t volatile* const AUX_MU_STAT_REG = (uint32_t*)AUX_MU_STAT;
uint32_t volatile* const AUX_MU_BAUD_REG = (uint32_t*)AUX_MU_BAUD;
//----------------------FIN MINI_UART------------------------------
