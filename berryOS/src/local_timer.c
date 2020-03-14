#include <local_timer.h>
#include <io/uart.h>
#include <proc/pcb.h>


/**
 * THIS STEPS APPLIES IN A SYSTEM WHERE THERE IS NOT VIRTUALIZATION SUPPORT
 * 
 * 1. Look into CNTP_CTL or CNTH_CTL or CNTV_CTL to enable or disable
 *    the corresponding timer (bit 0)
 * 2. You have to set the compare value for the corresponding timer
 *    CNTP_CVAL, CNTH_CVAL, CNTV_CVAL (carefull with CNTVOFF)
 * 3. Routing the IRQ and enabling IRQ of the corresponding core
 * To adjust the timer value you have to read the CNTFRQ register
*/
unsigned int current_time_value;
timer_selection_t core_timer_selected;

uint32_t local_timer_init(timer_selection_t local_timer, unsigned int time){
    long long unsigned int insert_value;
    //ENABLING TIMER
    write_CNTX_CTL(local_timer, 1);
    core_timer_selected = local_timer;

    //reading CNTFRQ
    insert_value = read_CNTFRQ();

    //updating TVAL
    uart_hex_puts(insert_value);
    insert_value = (insert_value >> 10)*time;
    uart_hex_puts(insert_value);
    current_time_value = insert_value;

    write_CNTX_TVAL(local_timer, insert_value);

    // ROUTING IRQ, we will assume just one core
    *(volatile uint32_t*)CORE0_L_TIMER_INT_CTL = 0x8;
    return 0;
}

uint64_t read_CNTPCT(void){
    uint64_t cntpct_value;

    asm volatile("mrc p15, #0, %0, c14, c2, #1": "=r"(cntpct_value):);

    return cntpct_value;
}

uint32_t read_CNTFRQ(void){
    uint32_t cntfrq_value;

    asm volatile("mrc p15, #0, %0, c14, c0, #0": "=r"(cntfrq_value):);

    return cntfrq_value;
}

uint64_t read_CNTX_CVAL(timer_selection_t local_timer_select){
    uint64_t cval_value;

    switch (local_timer_select){
        case PHYSICAL_SYS:
            asm volatile("mrc p15, #2, %0, c14, c14, #1": "=r"(cval_value):);
            break;
        case VIRTUAL_SYS:
            asm volatile("mrc p15, #3, %0, c14, c14, #1": "=r"(cval_value):);
            break;
        case PHYSICAL_HIPER:
            asm volatile("mrc p15, #6, %0, c14, c14, #1": "=r"(cval_value):);
            break;
        default:
            cval_value = -1;
            break;
    }

    return cval_value;
}


uint32_t read_CNTX_CTL(timer_selection_t local_timer_select){
    uint32_t ctl_value;

    switch (local_timer_select){
        case PHYSICAL_SYS:
            asm volatile("mrc p15, #2, %0, c14, c2, #1": "=r"(ctl_value):);
            break;
        case VIRTUAL_SYS:
            asm volatile("mrc p15, #0, %0, c14, c3, #1": "=r"(ctl_value):);
            break;
        case PHYSICAL_HIPER:
            asm volatile("mrc p15, #4, %0, c14, c2, #1": "=r"(ctl_value):);
            break;
        default:
            ctl_value = -1;
            break;
    }

    return ctl_value;
}


void write_CNTX_CVAL(timer_selection_t local_timer_select, uint64_t compare){
    uint32_t higher = (compare & 0xFFFFFFFF00000000) >> 32;
    uint32_t lower =   compare & 0x00000000FFFFFFFF;
    switch (local_timer_select){
        case PHYSICAL_SYS:
            asm volatile("mcrr p15, #2, %0, %1, c14": :"r"(lower), "r"(higher));
            break;
        case VIRTUAL_SYS:
            asm volatile("mcrr p15, #3, %0, %1, c14": :"r"(lower), "r"(higher));
            break;
        case PHYSICAL_HIPER:
            asm volatile("mcrr p15, #6, %0, %1, c14": :"r"(lower), "r"(higher));
            break;
    }
}

void write_CNTX_CTL(timer_selection_t local_timer_select, uint32_t flags){

    switch (local_timer_select){
        case PHYSICAL_SYS:
            asm volatile("mcr p15, #0, %0, c14, c2, #1": :"r"(flags));
            break;
        case VIRTUAL_SYS:
            asm volatile("mcr p15, #0, %0, c14, c3, #1": :"r"(flags));
            break;
        case PHYSICAL_HIPER:
            asm volatile("mcr p15, #4, %0, c14, c2, #1": :"r"(flags));
            break;
    }
}

void write_CNTX_TVAL(timer_selection_t local_timer_select, uint32_t value){
    switch (local_timer_select){
        case PHYSICAL_SYS:
            asm volatile("mcr p15, #0, %0, c14, c2, #0": :"r"(value));
            break;
        case VIRTUAL_SYS:
            asm volatile("mcr p15, #0, %0, c14, c3, #0": :"r"(value));
            break;
        case PHYSICAL_HIPER:
            asm volatile("mcr p15, #4, %0, c14, c2, #0": :"r"(value));
            break;
    }
}


/* STANDARD IRQ TIMER HANDLER */

//register like arm timer interrupt
void local_timer_handler(void) {
    /* uint32_t value2;
    uint32_t memory = *(volatile uint32_t*)CORE0_L_INT_SRC;
    uart_putln("Receive a generic timer interrupt");
    uart_puts("Value of core0 int source: ");
    asm volatile("ldr %0, [%1]" : "=r"(value2) : "r"(memory));
    value2 &= ~(0x8);
    uart_hex_puts(value2);
    uart_putln(""); */
    
    //uart_putln("\r\nChanging process!");
    schedule();
}


void local_timer_clearer(void) {
    //update the counter cval
    write_CNTX_TVAL(core_timer_selected, current_time_value);
}
