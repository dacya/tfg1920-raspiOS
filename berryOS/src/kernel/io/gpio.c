#include <io/gpio.h>

void set_pin_function(unsigned int pin, unsigned int fun_sel) {
    if (pin > 53 || fun_sel > 0b111) {
        return;
    }

    volatile unsigned* reg_obj;

    int reg_num = pin / 10;
    switch (reg_num) {
        case 0:
            reg_obj = GPFSEL0;
            break;
        case 1:
            reg_obj = GPFSEL1;
            break;
        case 2:
            reg_obj = GPFSEL2;
            break;
        case 3:
            reg_obj = GPFSEL3;
            break;
        case 4:
            reg_obj = GPFSEL4;
            break;
        case 5:
            reg_obj = GPFSEL5;
            break;
    }

    *reg_obj |= fun_sel << ((pin % 10) * 3)    
}

void set_pin_as_output(unsigned int pin) {
    if (pin > 53) {
        return;
    }

    volatile unsigned* reg_set;

    int reg_num = pin / 32;
    switch (reg_num) {
        case 0:
            reg_set = GPSET0;
            break;
        case 1:
            reg_set = GPSET1;
            break;
    }

    *reg_set |= 1 << (pin % 32);

}