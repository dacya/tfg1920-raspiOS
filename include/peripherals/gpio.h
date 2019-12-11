/**
 * This library is intended to make easier the interaction with
 * the gpio pins/ports. Currently it's compatible with raspi 2 and 3.
 * 
 * Tomás Golomb Durán
 * Alejandro Cancelo Correia
 * 
 * Nov 25, 2019
 */

#ifndef _GPIO_H
#define _GPIO_H

#include <stdint.h>
#include <peripherals/base.h>

/**
 * @dependency
 *
 * Base address for the gpio registers, currently supports pi2 and pi3.
 * For future devices this address may change.
 */
#define GPIO_BASE   (PHYSICAL_PBASE + 0x00200000)

//GPIO function select (registers 0 to 9)
#define GPFSEL0     ((volatile uint32_t*)(GPIO_BASE + 0x000))
//GPIO function select (registers 10 to 19)
#define GPFSEL1     ((volatile uint32_t*)(GPIO_BASE + 0x004))
//GPIO function select (registers 20 to 29)
#define GPFSEL2     ((volatile uint32_t*)(GPIO_BASE + 0x008))
//GPIO function select (registers 30 to 39)
#define GPFSEL3     ((volatile uint32_t*)(GPIO_BASE + 0x00C))
//GPIO function select (registers 40 to 49)
#define GPFSEL4     ((volatile uint32_t*)(GPIO_BASE + 0x010))
//GPIO function select (registers 50 to 53)
#define GPFSEL5     ((volatile uint32_t*)(GPIO_BASE + 0x014))
//GPIO pin output set (GPIO pin 0-31)
#define GPSET0      ((volatile uint32_t*)(GPIO_BASE + 0x01C))
//GPIO pin output set (GPIOpin 32-53)
#define GPSET1      ((volatile uint32_t*)(GPIO_BASE + 0x020))
//GPIO pin output clear (GPIO pin 0-31)
#define GPCLR0      ((volatile uint32_t*)(GPIO_BASE + 0x028))
//GPIO pin output clear (GPIOpin 32-53)
#define GPCLR1      ((volatile uint32_t*)(GPIO_BASE + 0x02C))
//GPIO pin level (GPIO pin 0-31)
#define GPLEV0      ((volatile uint32_t*)(GPIO_BASE + 0x034))
//GPIO pin level (GPIO pin 32-53)
#define GPLEV1      ((volatile uint32_t*)(GPIO_BASE + 0x038))
//GPIO pin event detect status 0 (GPIO pin 0-31)
#define GPEDS0      ((volatile uint32_t*)(GPIO_BASE + 0x040))
//GPIO pin event detect status 0 (GPIO pin 32-53)
#define GPEDS1      ((volatile uint32_t*)(GPIO_BASE + 0x044))
//GPIO rising-edge pin detect enable (GPIO pin 0-31)
#define GPREN0      ((volatile uint32_t*)(GPIO_BASE + 0x04C))
//GPIO rising-edge pin detect enable (GPIO pin 32-53)
#define GPREN1      ((volatile uint32_t*)(GPIO_BASE + 0x050))
//GPIO falling-edge pin detect enable (GPIO pin 0-31)
#define GPFEN0      ((volatile uint32_t*)(GPIO_BASE + 0x058))
//GPIO falling-edge pin detect enable (GPIO pin 32-53)
#define GPFEN1      ((volatile uint32_t*)(GPIO_BASE + 0x05C))
//GPIO pin high detect enable (GPIO pin 0-31)
#define GPHEN0      ((volatile uint32_t*)(GPIO_BASE + 0x064))
//GPIO pin high detect enable (GPIO pin 32-53)
#define GPHEN1      ((volatile uint32_t*)(GPIO_BASE + 0x068))
//GPIO pin low detect enable (GPIO pin 0-31)
#define GPLEN0      ((volatile uint32_t*)(GPIO_BASE + 0x070))
//GPIO pin low detect enable (GPIO pin 32-53)
#define GPLEN1      ((volatile uint32_t*)(GPIO_BASE + 0x074))
//GPIO Pin Async. rising edge detect (GPIO pin 0-31)
#define GPAREN0     ((volatile uint32_t*)(GPIO_BASE + 0x07C))
//GPIO Pin Async. rising edge detect (GPIO pin 32-53)
#define GPAREN1     ((volatile uint32_t*)(GPIO_BASE + 0x080))
//GPIO Pin Async.falling edge detect (GPIO pin 0-31)
#define GPALEN0     ((volatile uint32_t*)(GPIO_BASE + 0x088))
//GPIO Pin Async.falling edge detect (GPIO pin 32-53)
#define GPALEN1     ((volatile uint32_t*)(GPIO_BASE + 0x08C))
//GPIO pin pull-up/down register
#define GPPUD       ((volatile uint32_t*)(GPIO_BASE + 0x094))
//GPIO pin pull-up/down enable clock (GPIO pin 0-31)
#define GPPUDCLK0   ((volatile uint32_t*)(GPIO_BASE + 0x098))
//GPIO pin pull-up/down enable clock (GPIO pin 32-53)
#define GPPUDCLK1   ((volatile uint32_t*)(GPIO_BASE + 0x09C))

#endif /*_GPIO_H*/