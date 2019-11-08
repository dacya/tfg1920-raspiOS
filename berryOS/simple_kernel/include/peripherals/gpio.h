#ifndef	_P_GPIO_H
#define	_P_GPIO_H

#include "peripherals/base.h"
//all defined in page 89 and forward

//GPIO function select (registers 0 to 9)
uint32_t volatile* const GPFSEL0 = (uint32_t *)(PBASE+0x00200000);
//GPIO function select (registers 10 to 19)
uint32_t volatile* const GPFSEL1 = (uint32_t *)(PBASE+0x00200004); 
//GPIO function select (registers 20 to 29)
uint32_t volatile* const GPFSEL2 = (uint32_t *)(PBASE+0x00200008);
//GPIO function select (registers 30 to 39)
uint32_t volatile* const GPFSEL3 = (uint32_t *)(PBASE+0x0020000C);
//GPIO function select (registers 40 to 49)
uint32_t volatile* const GPFSEL4 = (uint32_t *)(PBASE+0x00200010);
//GPIO function select (registers 50 to 53)
uint32_t volatile* const GPFSEL5 = (uint32_t *)(PBASE+0x00200014); 

//GPIO pin output set (GPIO pin 0-31)
uint32_t volatile* const GPSET0 = (uint32_t *)(PBASE+0x0020001C); 
//GPIO pin output set (GPIOpin 32-53)
uint32_t volatile* const GPSET1 = (uint32_t *)(PBASE+0x00200020); 

//GPIO pin output clear (GPIO pin 0-31)
uint32_t volatile* const GPCLR0 = (uint32_t *)(PBASE+0x00200028); 
//GPIO pin output clear (GPIOpin 32-53)
uint32_t volatile* const GPCLR1 = (uint32_t *)(PBASE+0x0020002C); 

//GPIO pin level (GPIO pin 0-31)
uint32_t volatile* const GPLEV0 = (uint32_t *)(PBASE+0x00200034); 
//GPIO pin level (GPIO pin 32-53)
uint32_t volatile* const GPLEV1 = (uint32_t *)(PBASE+0x00200038); 

//GPIO pin event detect status 0 (GPIO pin 0-31)
uint32_t volatile* const GPEDS0 = (uint32_t *)(PBASE+0x00200040); 
//GPIO pin event detect status 0 (GPIO pin 32-53)
uint32_t volatile* const GPEDS1 = (uint32_t *)(PBASE+0x00200044); 

//GPIO rising-edge pin detect enable (GPIO pin 0-31)
uint32_t volatile* const GPREN0 = (uint32_t *)(PBASE+0x0020004C); 
//GPIO rising-edge pin detect enable (GPIO pin 32-53)
uint32_t volatile* const GPREN1 = (uint32_t *)(PBASE+0x00200050);

//GPIO falling-edge pin detect enable (GPIO pin 0-31)
uint32_t volatile* const GPFEN0 = (uint32_t *)(PBASE+0x00200058); 
//GPIO falling-edge pin detect enable (GPIO pin 32-53)
uint32_t volatile* const GPFEN1 = (uint32_t *)(PBASE+0x0020005C);

//GPIO pin high detect enable (GPIO pin 0-31)
uint32_t volatile* const GPHEN0 = (uint32_t *)(PBASE+0x02000064);
//GPIO pin high detect enable (GPIO pin 32-53)
uint32_t volatile* const GPHEN1 = (uint32_t *)(PBASE+0x02000068);

//GPIO pin low detect enable (GPIO pin 0-31)
uint32_t volatile* const GPLEN0 = (uint32_t *)(PBASE+0x02000070); 
//GPIO pin low detect enable (GPIO pin 32-53)
uint32_t volatile* const GPLEN1 = (uint32_t *)(PBASE+0x02000074);

//GPIO Pin Async. rising edge detect (GPIO pin 0-31)
uint32_t volatile* const GPAREN0 = (uint32_t *)(PBASE+0x0200007C); 
//GPIO Pin Async. rising edge detect (GPIO pin 32-53)
uint32_t volatile* const GPAREN1 = (uint32_t *)(PBASE+0x02000080);

//GPIO Pin Async.falling edge detect (GPIO pin 0-31)
uint32_t volatile* const GPALEN0 = (uint32_t *)(PBASE+0x02000088); 
//GPIO Pin Async.falling edge detect (GPIO pin 32-53)
uint32_t volatile* const GPALEN1 = (uint32_t *)(PBASE+0x0200008C);

//GPIO pin pull-up/down register
uint32_t volatile* const GPPUD = (uint32_t *)(PBASE+0x00200094); 
//GPIO pin pull-up/down enable clock (GPIO pin 0-31)
uint32_t volatile* const GPPUDCLK0 = (uint32_t *)(PBASE+0x00200098); 
//GPIO pin pull-up/down enable clock (GPIO pin 32-53)
uint32_t volatile* const GPPUDCLK1 = (uint32_t *)(PBASE+0x0020009C); 
#endif  /*_P_GPIO_H */
