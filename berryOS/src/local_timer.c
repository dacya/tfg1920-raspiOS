#include <local_timer.h>

extern void _local_timer_init(void);

void local_timer_init(void)
{
    /**
     * THIS STEPS APPLIES IN A SYSTEM WHERE THERE IS NOT VIRTUALIZATION SUPPORT
     * 
     * 1. Look into CNTKCTL register if you need
     * 2. Look into CNTP_CTL or CNTH_CTL or CNTV_CTL to enable or disable
     *    the corresponding timer (bit 0)
     * 3. You have to set the compare value for the corresponding timer
     *    CNTP_CVAL, CNTH_CVAL, CNTV_CVAL if needed
     * 4. It should be in boot.S but you have to initialize the counter 
     *    frequency register, CNTFRQ
     * 5. Putting the corresponding TVAL register to a right value
     * 6. Routing the IRQ and enabling IRQ of the corresponding core
    */
    _local_timer_init();
    // ROUTING IRQ
    *(volatile uint32_t*)CORE0_L_TIMER_INT_CTL = 0x8;
}
