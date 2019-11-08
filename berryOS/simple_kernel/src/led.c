#include <stdint.h>
#include "../include/peripherals/gpio.h"
#include "../include/led.h"

uint32_t volatile* const GPFSEL2_REG = (uint32_t*) GPFSEL2;
uint32_t volatile* const GPSET0_REG = (uint32_t*) GPSET0;
uint32_t volatile* const GPCLR0_REG = (uint32_t*) GPCLR0;

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