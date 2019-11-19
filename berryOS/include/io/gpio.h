/**
 * This library is intended to make easier the interaction with
 * the gpio pins/ports. Currently it's compatible with raspi 2 and 3.
 * 
 * Tomás Golomb Durán - Nov. 12, 2019
 */

#ifndef _GPIO_H
#define _GPIO_H

/**
 * @dependency
 *
 * Base address for the gpio registers, currently supports pi2 and pi3.
 * For future devices this address may change.
 */
#define GPIO_BASE   ((volatile unsigned*)(0x3F200000))

#define GPFSEL0     ((volatile unsigned*)(GPIO_BASE + 0x000))
#define GPFSEL1     ((volatile unsigned*)(GPIO_BASE + 0x004))
#define GPFSEL2     ((volatile unsigned*)(GPIO_BASE + 0x008))
#define GPFSEL3     ((volatile unsigned*)(GPIO_BASE + 0x00C))
#define GPFSEL4     ((volatile unsigned*)(GPIO_BASE + 0x010))
#define GPFSEL5     ((volatile unsigned*)(GPIO_BASE + 0x014))
#define GPSET0      ((volatile unsigned*)(GPIO_BASE + 0x01C))
#define GPSET1      ((volatile unsigned*)(GPIO_BASE + 0x020))
#define GPCLR0      ((volatile unsigned*)(GPIO_BASE + 0x028))
#define GPCLR1      ((volatile unsigned*)(GPIO_BASE + 0x02C))
#define GPLEV0      ((volatile unsigned*)(GPIO_BASE + 0x034))
#define GPLEV1      ((volatile unsigned*)(GPIO_BASE + 0x038))
#define GPEDS0      ((volatile unsigned*)(GPIO_BASE + 0x040))
#define GPEDS1      ((volatile unsigned*)(GPIO_BASE + 0x044))
#define GPREN0      ((volatile unsigned*)(GPIO_BASE + 0x04C))
#define GPREN1      ((volatile unsigned*)(GPIO_BASE + 0x050))
#define GPFEN0      ((volatile unsigned*)(GPIO_BASE + 0x058))
#define GPFEN1      ((volatile unsigned*)(GPIO_BASE + 0x05C))
#define GPHEN0      ((volatile unsigned*)(GPIO_BASE + 0x064))
#define GPHEN1      ((volatile unsigned*)(GPIO_BASE + 0x068))
#define GPLEN0      ((volatile unsigned*)(GPIO_BASE + 0x070))
#define GPLEN1      ((volatile unsigned*)(GPIO_BASE + 0x074))
#define GPAREN0     ((volatile unsigned*)(GPIO_BASE + 0x07C))
#define GPAREN1     ((volatile unsigned*)(GPIO_BASE + 0x080))
#define GPAFEN0     ((volatile unsigned*)(GPIO_BASE + 0x088))
#define GPAFEN1     ((volatile unsigned*)(GPIO_BASE + 0x08C))
#define GPPUD       ((volatile unsigned*)(GPIO_BASE + 0x094))
#define GPPUDCLK0   ((volatile unsigned*)(GPIO_BASE + 0x098))
#define GPPUDCLK1   ((volatile unsigned*)(GPIO_BASE + 0x09C))

/**
 * Defines the operation for a GPIO pin. 
 *
 * @param pin 1 out of the 54 pins {0 - 53}
 * @param fun_sel The operation type for the pin. There are up to 8 alternatives for each pin, 
 * however not every pin has a defined function for the 8 alternatives, these are listed in
 * the Alternative Function Assignments Table provided in the ARM Peripherals documentation.
 * @return void
 */
void pin_set_function(unsigned int pin, unsigned int fun_sel);

/**
 * Sets the desired pin as output.
 *
 * @param pin 1 out of the 54 pins {0 - 53}
 * @return void
 */
void pin_set_as_output(unsigned int pin);

/**
 * Clears the output signaling in the give pin
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @return void
 */
void pin_clear_output(unsigned int pin);

/**
 * Get the current pin level
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @return 0 if low 1 if high
 */
unsigned int pin_get_level(unsigned int pin);

/**
 * Get the current status for the programmed event in the given pin
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @param clear 0 to prevent the flag from being cleared
 * @return 1 if the event has been detected, 0 otherwise
 */
unsigned int pin_get_event_status_flag(unsigned int pin, int clear);

/**
 * Clears the event status flag of the given pin.
 * The cleaning will take effect only if the 
 * status has been detected.
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @return void
 */
void pin_clear_event_status_flag(unsigned int pin);

/**
 * Enables the event of a rising edge signal (0 to 1)
 * over a pin. To know if the event has been
 * triggered use pin_get_event_status_flag function.
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @return void
 * @see pin_get_event_status_flag
 */
void pin_enable_rising_edge_event(unsigned int pin);

/**
 * Enables the event of a falling edge signal (1 to 0)
 * over a pin. To know if the event has been
 * triggered use pin_get_event_status_flag function.
 * 
 * @param pin 1 out of the 54 pins {0 - 53}
 * @return void
 * @see pin_get_event_status_flag
 */
void pin_enable_falling_edge_event(unsigned int pin);


#endif /* _GPIO_H */