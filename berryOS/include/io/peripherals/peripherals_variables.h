#ifndef _P_PERIPHERALS_VARIABLES_H
#define _P_PERIPHERALS_VARIABLES_H

#include <stdint.h>
#include <io/peripherals/gpio.h>
#include <io/peripherals/mini_uart.h>
#include <io/peripherals/uart.h>

//                      PL011 UART
#define UART0_DR_REG ((volatile unsigned *)(UART0_DR))
#define UART0_FR_REG ((volatile unsigned *)(UART0_FR))
#define UART0_CR_REG ((volatile unsigned *)(UART0_CR))
#define UART0_IBRD_REG ((volatile unsigned *)(UART0_IBRD))
#define UART0_FBRD_REG ((volatile unsigned *)(UART0_FBRD))
#define UART0_LCRH_REG ((volatile unsigned *)(UART0_LCRH))
#define UART0_ICR_REG ((volatile unsigned *)(UART0_ICR))
#define UART0_IMSC_REG ((volatile unsigned *)(UART0_IMSC))
//----------------------FIN PL011 UART-----------------------------


//                      GPIO
#define GPFSEL0_REG ((volatile unsigned *)(GPFSEL0))
#define GPFSEL1_REG ((volatile unsigned *)(GPFSEL1))
#define GPFSEL2_REG ((volatile unsigned *)(GPFSEL2))
#define GPFSEL3_REG ((volatile unsigned *)(GPFSEL3))
#define GPFSEL4_REG ((volatile unsigned *)(GPFSEL4))
#define GPFSEL5_REG ((volatile unsigned *)(GPFSEL5))

#define GPSET0_REG ((volatile unsigned *)(GPSET0))
#define GPSET1_REG ((volatile unsigned *)(GPSET1))

#define GPCLR0_REG ((volatile unsigned *)(GPCLR0))
#define GPCLR1_REG ((volatile unsigned *)(GPCLR1))

#define GPLEV0_REG ((volatile unsigned *)(GPLEV0))
#define GPLEV1_REG ((volatile unsigned *)(GPLEV1))

#define GPEDS0_REG ((volatile unsigned *)(GPEDS0))
#define GPEDS1_REG ((volatile unsigned *)(GPEDS1))

#define GPREN0_REG ((volatile unsigned *)(GPREN0))
#define GPREN1_REG ((volatile unsigned *)(GPREN1))

#define GPFEN0_REG ((volatile unsigned *)(GPFEN0))
#define GPFEN1_REG ((volatile unsigned *)(GPFEN1))

#define GPHEN0_REG ((volatile unsigned *)(GPHEN0))
#define GPHEN1_REG ((volatile unsigned *)(GPHEN1))

#define GPLEN0_REG ((volatile unsigned *)(GPLEN0))
#define GPLEN1_REG ((volatile unsigned *)(GPLEN1))

#define GPAREN0_REG ((volatile unsigned *)(GPAREN0))
#define GPAREN1_REG ((volatile unsigned *)(GPAREN1))

#define GPALEN0_REG ((volatile unsigned *)(GPALEN0))
#define GPALEN1_REG ((volatile unsigned *)(GPALEN1))

#define GPPUD_REG ((volatile unsigned *)(GPPUD))
#define GPPUDCLK0_REG ((volatile unsigned *)(GPPUDCLK0))
#define GPPUDCLK1_REG ((volatile unsigned *)(GPPUDCLK1))
//----------------------FIN GPIO-----------------------------------


//                      MINI_UART
#define AUX_IRQ_REG ((volatile unsigned *)(AUX_IRQ))
#define AUX_ENABLES_REG ((volatile unsigned *)(AUX_ENABLES))

#define AUX_MU_IO_REG ((volatile unsigned *)(AUX_MU_IO))
#define AUX_MU_IER_REG ((volatile unsigned *)(AUX_MU_IER))
#define AUX_MU_IIR_REG ((volatile unsigned *)(AUX_MU_IIR))
#define AUX_MU_LCR_REG ((volatile unsigned *)(AUX_MU_LCR))
#define AUX_MU_MCR_REG ((volatile unsigned *)(AUX_MU_MCR))
#define AUX_MU_LSR_REG ((volatile unsigned *)(AUX_MU_LSR))
#define AUX_MU_MSR_REG ((volatile unsigned *)(AUX_MU_MSR))
#define AUX_MU_SCRATCH_REG ((volatile unsigned *)(AUX_MU_SCRATCH))
#define AUX_MU_CNTL_REG ((volatile unsigned *)(AUX_MU_CNTL))
#define AUX_MU_STAT_REG ((volatile unsigned *)(AUX_MU_STAT))
#define AUX_MU_BAUD_REG ((volatile unsigned *)(AUX_MU_BAUD))

#define AUX_SPI1_CNTL0_REG ((volatile unsigned *)(AUX_SPI1_CNTL0))
#define AUX_SPI1_CNTL1_REG ((volatile unsigned *)(AUX_SPI1_CNTL1))
#define AUX_SPI1_STAT_REG ((volatile unsigned *)(AUX_SPI1_STAT))
#define AUX_SPI1_IO_REG ((volatile unsigned *)(AUX_SPI1_IO))
#define AUX_SPI1_PEEK_REG ((volatile unsigned *)(AUX_SPI1_PEEK))

#define AUX_SPI2_CNTL0_REG ((volatile unsigned *)(AUX_SPI2_CNTL0))
#define AUX_SPI2_CNTL1_REG ((volatile unsigned *)(AUX_SPI2_CNTL1))
#define AUX_SPI2_STAT_REG ((volatile unsigned *)(AUX_SPI2_STAT))
#define AUX_SPI2_IO_REG ((volatile unsigned *)(AUX_SPI2_IO))
#define AUX_SPI2_PEEK_REG ((volatile unsigned *)(AUX_SPI2_PEEK))

//----------------------FIN MINI_UART-----------------------------------

#endif