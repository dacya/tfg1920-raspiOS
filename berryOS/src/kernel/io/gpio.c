#include <io/gpio/gpio.h>

void pin_set_function(unsigned int pin, unsigned int fun_sel) {
    if (pin > 53 || fun_sel > 0b111) {
        return ;
    }

    volatile unsigned* reg_obj;

    switch (pin / 10) {
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

    *reg_obj |= fun_sel << ((pin % 10) * 3);
}

void pin_set_as_output(unsigned int pin) {
    if (pin > 53) {
        return ;
    }

    volatile unsigned* reg_set;

    switch (pin / 32) {
        case 0:
            reg_set = GPSET0;
            break;
        case 1:
            reg_set = GPSET1;
            break;
    }

    *reg_set |= 1 << (pin % 32);
}

void pin_clear_output(unsigned int pin) {
    if (pin > 53)
        return;

    volatile unsigned* reg;

    switch (pin / 32) {
        case 0:
            reg = GPCLR0;
            break;
        case 1:
            reg = GPCLR1;
            break;
    }

    *reg |= 1 << (pin % 32);
}

unsigned int pin_get_level(unsigned int pin) {
    if (pin > 53)
        return;
    
    volatile unsigned* reg;

    switch (pin / 32) {
        case 0:
            reg = GPLEV0;
            break;
        case 1:
            reg = GPLEV1;
            break;
    }

    return ((*reg) >> (pin % 32)) & 1;
}

unsigned int pin_get_event_status_flag(unsigned int pin, int clear) {
    if (pin > 53)
        return 0;
    
    volatile unsigned* reg;

    switch (pin / 32) {
        case 0:
            reg = GPEDS0;
            break;
        case 1:
            reg = GPEDS1;
            break;
    }

    unsigned int flag = ((*reg) >> (pin % 32)) & 1;

    if (clear)
        *reg |= 1 << (pin % 32);

    return flag;
}

void pin_clear_event_status_flag(unsigned int pin) {
    if (pin > 53)
        return ;
    
    volatile unsigned* reg;

    switch (pin / 32) {
        case 0:
            reg = GPEDS0;
            break;
        case 1:
            reg = GPEDS1;
            break;
    }
    
    *reg |= 1 << (pin % 32);
}

void pin_switch_rising_edge_event(unsigned int pin, int enable) {
    if (pin > 53)
        return ;
    
    volatile unsigned* reg;

    switch (pin / 32) {
        case 0:
            reg = GPREN0;
            break;
        case 1:
            reg = GPREN1;
            break;
    }
    
    if (enable) {
        *reg |= 1 << (pin % 32);
    } else {
        *reg &= ~(1 << (pin % 32));
    }
}

void pin_switch_falling_edge_event(unsigned int pin, int enable) {
    if (pin > 53)
        return ;
    
    volatile unsigned* reg;

    switch (pin / 32) {
        case 0:
            reg = GPFEN0;
            break;
        case 1:
            reg = GPFEN1;
            break;
    }
    
    if (enable) {
        *reg |= 1 << (pin % 32);
    } else {
        *reg &= ~(1 << (pin % 32));
    }
}