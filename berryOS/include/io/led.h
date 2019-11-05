#ifndef _P_LED_H
#define _P_LED_H

enum led_flag {
    HIGH,
    LOW
};

void led_init( void ); //the function takes no parameters
void led_set(enum led_flag value);

#endif