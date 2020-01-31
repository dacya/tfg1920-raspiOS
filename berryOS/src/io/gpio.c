#include <io/gpio.h>

void pin_set_function(unsigned int pin, pin_alt_funct fun_sel) {
    if (pin > 53) {
        return ;
    }

    volatile uint32_t* reg_obj;

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

    volatile uint32_t* reg_set;

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

    volatile uint32_t* reg;

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

int pin_get_level(unsigned int pin) {
    if (pin > 53)
        return -1;
    
    volatile uint32_t* reg;

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
    
    volatile uint32_t* reg;

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
    
    volatile uint32_t* reg;

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
    
    volatile uint32_t* reg;

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
        pin_clear_event_status_flag(pin);
        *reg &= ~(1 << (pin % 32));
    }
}

void pin_switch_falling_edge_event(unsigned int pin, int enable) {
    if (pin > 53)
        return ;
    
    volatile uint32_t* reg;

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
        pin_clear_event_status_flag(pin);
        *reg &= ~(1 << (pin % 32));
    }
}

void pin_switch_high_event(unsigned int pin, int enable) {
    if (pin > 53)
        return ;
    
    volatile uint32_t* reg;

    switch (pin / 32) {
        case 0:
            reg = GPHEN0;
            break;
        case 1:
            reg = GPHEN1;
            break;
    }
    
    if (enable) {
        *reg |= 1 << (pin % 32);
    } else {
        pin_clear_event_status_flag(pin);
        *reg &= ~(1 << (pin % 32));
    }
}

void pin_switch_low_event(unsigned int pin, int enable) {
    if (pin > 53)
        return ;
    
    volatile uint32_t* reg;

    switch (pin / 32) {
        case 0:
            reg = GPLEN0;
            break;
        case 1:
            reg = GPLEN1;
            break;
    }
    
    if (enable) {
        *reg |= 1 << (pin % 32);
    } else {
        pin_clear_event_status_flag(pin);
        *reg &= ~(1 << (pin % 32));
    }
}

void pin_switch_async_rising_event(unsigned int pin, int enable) {
    if (pin > 53)
        return ;
    
    volatile uint32_t* reg;

    switch (pin / 32) {
        case 0:
            reg = GPAREN0;
            break;
        case 1:
            reg = GPAREN1;
            break;
    }
    
    if (enable) {
        *reg |= 1 << (pin % 32);
    } else {
        pin_clear_event_status_flag(pin);
        *reg &= ~(1 << (pin % 32));
    }
}

void pin_switch_async_falling_event(unsigned int pin, int enable) {
    if (pin > 53)
        return ;
    
    volatile uint32_t* reg;

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
        pin_clear_event_status_flag(pin);
        *reg &= ~(1 << (pin % 32));
    }
}

void switch_pud_control(unsigned int state) {
    *GPPUD &= ~0b11;
    *GPPUD |= state;
}

void pin_switch_pud(unsigned int pin, unsigned int state) {
    if (pin > 53 || state > 0b10)
        return ;
    
    // 1 - sets the control signal
    switch_pud_control(state);

    // 2 - 150 cycles for set-up
    delay(150);

    // 3 - enables the pin pud
    volatile uint32_t* reg;
    switch (pin / 32) {
        case 0: reg = GPPUDCLK0; break;
        case 1: reg = GPPUDCLK1; break;
    }
    
    *reg |= 1 << (pin % 32);

    // 4 - 150 cycles for set-up
    delay(150);

    // 5 - disables the pud control line
    switch_pud_control(0b00);

    // 5 - removes the flag for the pud clock wait (?)
    *reg &= ~(1 << (pin % 32));
}

