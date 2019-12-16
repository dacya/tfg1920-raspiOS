/*#include <stdint.h>
#include <peripherals/gpio.h>
#include <io/led.h>

void led_init( void ){
    uint32_t selector = *(GPFSEL2);

    selector |= (1<<17);
    *(GPFSEL2) = selector; //we declare the GPIO pin 17 as output
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
    
    *(GPSET0) = setSr;
    *(GPCLR0) = clearSr;
}*/