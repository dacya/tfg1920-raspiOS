#include "utils.h"
#include "peripherals/gpio.h"
#include "led.h"

void led_init( void ){
    unsigned int selector = 0;

    selector |= (1<<21);
    put32(GPFSEL2, selector); //we declare the GPIO pin 21 as output

}

void led_set(enum led_flag value){
    unsigned int setSr = 0;
    unsigned int clearSr = 0;

    if(value == HIGH){
        setSr |= (1<<21);
        clearSr = 0;
    }
    else{
        setSr = 0;
        clearSr |= (1<<21);
    }
    
    put32(GPSET0, setSr);
    put32(GPCLR0, clearSr);
}