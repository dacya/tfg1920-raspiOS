#ifndef _GPIO_H
#define _GPIO_H

/**
    This library is intended to make easier the interaction with
    the gpio pins/ports. Currently it's compatible with raspi 2 and 3.
 */

#define GPFSEL0     ((volatile unsigned*)(0x7E200000)
#define GPFSEL1     ((volatile unsigned*)(0x7E200004)
#define GPFSEL2     ((volatile unsigned*)(0x7E200008)
#define GPFSEL3     ((volatile unsigned*)(0x7E20000C)
#define GPFSEL4     ((volatile unsigned*)(0x7E200010)
#define GPFSEL5     ((volatile unsigned*)(0x7E200014)
#define GPSET0      ((volatile unsigned*)(0x7E20001C)
#define GPSET1      ((volatile unsigned*)(0x7E200020)
#define GPCLR0      ((volatile unsigned*)(0x7E200028)
#define GPCLR1      ((volatile unsigned*)(0x7E20002C)
#define GPLEV0      ((volatile unsigned*)(0x7E200034)
#define GPLEV1      ((volatile unsigned*)(0x7E200038)
#define GPEDS0      ((volatile unsigned*)(0x7E200040)
#define GPEDS1      ((volatile unsigned*)(0x7E200044)
#define GPREN0      ((volatile unsigned*)(0x7E20004C)
#define GPREN1      ((volatile unsigned*)(0x7E200050)
#define GPFEN0      ((volatile unsigned*)(0x7E200058)
#define GPFEN1      ((volatile unsigned*)(0x7E20005C)
#define GPHEN0      ((volatile unsigned*)(0x7E200064)
#define GPHEN1      ((volatile unsigned*)(0x7E200068)
#define GPLEN0      ((volatile unsigned*)(0x7E200070)
#define GPLEN1      ((volatile unsigned*)(0x7E200074)
#define GPAREN0     ((volatile unsigned*)(0x7E20007C)
#define GPAREN1     ((volatile unsigned*)(0x7E200080)
#define GPAFEN0     ((volatile unsigned*)(0x7E200088)
#define GPAFEN1     ((volatile unsigned*)(0x7E20008C)
#define GPPUD       ((volatile unsigned*)(0x7E200094)
#define GPPUDCLK0   ((volatile unsigned*)(0x7E200098)
#define GPPUDCLK1   ((volatile unsigned*)(0x7E20009C)

/**
 * Defines the operation for a GPIO pin. 
 *
 * @param pin One out of the 54 pins {0 - 53}
 * @param fun_sel The operation type for the pin. There are up to 8 alternatives for each pin, 
 * however not every pin has a defined function for the 8 alternatives, these are listed in
 * the Alternative Function Assignments Table provided in the ARM Peripherals documentation.
 * @return void
 */
void set_pin_function(unsigned int pin, unsigned int fun_sel);

/**
 * Sets the desired pin as output.
 *
 * @param pin One out of the 54 pins {0 - 53}
 * @return void
 */
void set_pin_as_output(unsigned int pin);

/**
 *
 */
void clear_pin_output(unsigned int pin);



#endif /* _GPIO_H */