#ifndef	_P_GPIO_H
#define	_P_GPIO_H

#include "base.h"
//all defined in page 89 and forward

//GPIO function select (registers 0 to 9)
 #define GPFSEL0 (PBASE+0x00200000)
//GPIO function select (registers 10 to 19)
 #define GPFSEL1 (PBASE+0x00200004) 
//GPIO function select (registers 20 to 29)
 #define GPFSEL2 (PBASE+0x00200008)
//GPIO function select (registers 30 to 39)
 #define GPFSEL3 (PBASE+0x0020000C)
//GPIO function select (registers 40 to 49)
 #define GPFSEL4 (PBASE+0x00200010)
//GPIO function select (registers 50 to 53)
 #define GPFSEL5 (PBASE+0x00200014) 

//GPIO pin output set (GPIO pin 0-31)
 #define GPSET0 (PBASE+0x0020001C) 
//GPIO pin output set (GPIOpin 32-53)
 #define GPSET1 (PBASE+0x00200020) 

//GPIO pin output clear (GPIO pin 0-31)
 #define GPCLR0 (PBASE+0x00200028) 
//GPIO pin output clear (GPIOpin 32-53)
 #define GPCLR1 (PBASE+0x0020002C) 

//GPIO pin level (GPIO pin 0-31)
 #define GPLEV0 (PBASE+0x00200034) 
//GPIO pin level (GPIO pin 32-53)
 #define GPLEV1 (PBASE+0x00200038) 

//GPIO pin event detect status 0 (GPIO pin 0-31)
 #define GPEDS0 (PBASE+0x00200040) 
//GPIO pin event detect status 0 (GPIO pin 32-53)
 #define GPEDS1 (PBASE+0x00200044) 

//GPIO rising-edge pin detect enable (GPIO pin 0-31)
 #define GPREN0 (PBASE+0x0020004C) 
//GPIO rising-edge pin detect enable (GPIO pin 32-53)
 #define GPREN1 (PBASE+0x00200050)

//GPIO falling-edge pin detect enable (GPIO pin 0-31)
 #define GPFEN0 (PBASE+0x00200058) 
//GPIO falling-edge pin detect enable (GPIO pin 32-53)
 #define GPFEN1 (PBASE+0x0020005C)

//GPIO pin high detect enable (GPIO pin 0-31)
 #define GPHEN0 (PBASE+0x02000064)
//GPIO pin high detect enable (GPIO pin 32-53)
 #define GPHEN1 (PBASE+0x02000068)

//GPIO pin low detect enable (GPIO pin 0-31)
 #define GPLEN0 (PBASE+0x02000070) 
//GPIO pin low detect enable (GPIO pin 32-53)
 #define GPLEN1 (PBASE+0x02000074)

//GPIO Pin Async. rising edge detect (GPIO pin 0-31)
 #define GPAREN0 (PBASE+0x0200007C) 
//GPIO Pin Async. rising edge detect (GPIO pin 32-53)
 #define GPAREN1 (PBASE+0x02000080)

//GPIO Pin Async.falling edge detect (GPIO pin 0-31)
 #define GPALEN0 (PBASE+0x02000088) 
//GPIO Pin Async.falling edge detect (GPIO pin 32-53)
 #define GPALEN1 (PBASE+0x0200008C)

//GPIO pin pull-up/down register
 #define GPPUD (PBASE+0x00200094) 
//GPIO pin pull-up/down enable clock (GPIO pin 0-31)
 #define GPPUDCLK0 (PBASE+0x00200098) 
//GPIO pin pull-up/down enable clock (GPIO pin 32-53)
 #define GPPUDCLK1 (PBASE+0x0020009C) 
#endif  /*_P_GPIO_H */
