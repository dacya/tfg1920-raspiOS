#include <local_timer.h>
#include <io/uart.h>
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

timer_selection_t core_timer_selected;

uint32_t local_timer_init(timer_selection_t local_timer, uint32_t time){
    uint64_t insert_value;
    //ENABLING TIMER
    write_CNTX_CTL(local_timer, 1);
    core_timer_selected = local_timer;

    //reading CNTPCT
    //insert_value = read_CNTPCT();
    insert_value = read_CNTFRQ();
    uart_puts("After reading cntpct\r\n");
    //updating CVAL
    //insert_value += time;

    //write_CNTX_CVAL(local_timer, insert_value);
    write_CNTX_TVAL(local_timer, insert_value);
    uart_puts("After writing cval\r\n");

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
/*
.globl _local_timer_init
     * THIS STEPS APPLIES IN A SYSTEM WHERE THERE IS NOT VIRTUALIZATION SUPPORT
     * 
     * 1. Look into CNTKCTL register if you need
     * 2. Look into CNTP_CTL or CNTH_CTL or CNTV_CTL to enable or disable
     *    the corresponding timer (bit 0)
     * 3. You have to set the compare value for the corresponding timer
     *    CNTP_CVAL, CNTH_CVAL, CNTV_CVAL if needed
     * 4. Putting the corresponding TVAL register to a right value
     * 5. Routing the IRQ and enabling IRQ of the corresponding core
    
_local_timer_init:
    // ENABLING TIMER
    //mrc p15, #0, r0, c14, c3, #1 //we obtain CNTV_CTL register value
    //orr r1, r1, #1 //we enable virtual timer
    mov r0, #1
    mcr p15, #0, r0, c14, c3, #1
    // SETTING FREQUENCY TIMER
    //we don't need this right now
    // SETTING TVAL REGISTER (virtual)
    mrc p15, #0, r0, c14, c0, #0 //we obtain CNTFRQ
    mcr p15, #0, r0, c14, c3, #0 //1 irq per second
    mov pc, lr
*/