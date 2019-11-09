#include <stdint.h>
#include "../include/io/peripherals/peripherals_variables.h"
#include "../include/io/led.h"

void led_init( void ){
    uint32_t selector = *(GPFSEL2_REG);

    selector |= (1<<17);
    *(GPFSEL2_REG) = selector; //we declare the GPIO pin 17 as output
}

void led_set(enum led_flag value){
    uint32_t setSr = 0;
    uint32_t clearSr = 0;

    if(value == HIGH){
        setSr |= (1<<21);
        clearSr = 0;
    }
    else{
        setSr = 0;
        clearSr |= (1<<21);
    }
    
    *(GPSET0_REG) = setSr;
    *(GPCLR0_REG) = clearSr;
}