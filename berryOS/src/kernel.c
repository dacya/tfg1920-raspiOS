#include <stddef.h>
#include <stdint.h>

#include <io/uart.h>
#include <interrupts.h>
#include <io/gpu.h>

extern void io_halt();

static interrupt_registers_t* interrupt_regs;

// array of pointers to handlers functions
static interrupt_handler_f_ptr handlers[NUM_P_IRQS];
// array of pointer to clearers functions
static interrupt_clearer_f_ptr clearers[NUM_P_IRQS];

void bzero(void * dest, int bytes) {
    char * d = dest;
    while (bytes--) {
        *d++ = 0;
    }
}

void interrupts_init(void)
{
    interrupt_regs = (interrupt_registers_t *)INTERRUPTS_P_PENDING;
	bzero(handlers, sizeof(interrupt_handler_f_ptr) * NUM_P_IRQS);
	bzero(clearers, sizeof(interrupt_clearer_f_ptr) * NUM_P_IRQS);
    // disable all interrupts
	interrupt_regs->irq_base_int_disable = 0xffffffff; 
	interrupt_regs->irq_disable1 = 0xffffffff;
	interrupt_regs->irq_disable2 = 0xffffffff;
}

void register_irq_handler(irq_number_t irq_num, 
                          interrupt_handler_f_ptr handler_to_register, 
                          interrupt_clearer_f_ptr clearer_to_register)
{
    uint32_t pos;
    if(IRQ_P_IS_BASIC(irq_num))
    {
        pos = irq_num - 64; //to get a number between 0 and 7
        handlers[irq_num] = handler_to_register;
        clearers[irq_num] = clearer_to_register;
        interrupt_regs->irq_base_int_disable &= ~(1 << pos);
        interrupt_regs->irq_base_int_enable |= (1 << pos);
    }
    else if (IRQ_P_IS_GPU2(irq_num))
    {
        pos = irq_num - 32; // to get a number between 0 and 31
        handlers[irq_num] = handler_to_register;
        clearers[irq_num] = clearer_to_register;
        interrupt_regs->irq_disable2 &= ~(1 << pos);
        interrupt_regs->irq_enable2 |= (1 << pos);
    }
    else if (IRQ_P_IS_GPU1(irq_num))
    {
        handlers[irq_num] = handler_to_register;
        clearers[irq_num] = clearer_to_register;
        interrupt_regs->irq_disable1 &= ~(1 << irq_num);
        interrupt_regs->irq_enable1 |= (1 << irq_num);
    }
    else
    {
        uart_puts("HANDLER ERROR: Couldn't register the handler & clearer!\r\n");
    }
    
}

void unregister_irq_isr(irq_number_t irq_num)
{
    uint32_t pos;
    if(IRQ_P_IS_BASIC(irq_num))
    {
        pos = irq_num - 64; //to get a number between 0 and 7
        handlers[irq_num] = 0;
        clearers[irq_num] = 0;
        interrupt_regs->irq_base_int_enable &= ~(1 << pos);
        interrupt_regs->irq_base_int_disable |= (1 << pos);
    }
    else if (IRQ_P_IS_GPU1(irq_num))
    {
        pos = irq_num - 32; // to get a number between 0 and 31
        handlers[irq_num] = 0;
        clearers[irq_num] = 0;
        interrupt_regs->irq_base_int_enable &= ~(1 << pos);
        interrupt_regs->irq_base_int_disable |= (1 << pos);
    }
    else if (IRQ_P_IS_GPU2(irq_num))
    {
        handlers[irq_num] = NULL;
        clearers[irq_num] = NULL;
        interrupt_regs->irq_base_int_enable &= ~(1 << irq_num);
        interrupt_regs->irq_base_int_disable |= (1 << irq_num);
    }
    else
    {
        uart_puts("HANDLER ERROR: Couldn't remove the handler and clearer!\r\n");
    }
}

//Custom irq handler
void irq_c_handler(void) {
    //the cpu reaches this function with the IRQ exceptions disabled
    for(int i = 0; i < NUM_P_IRQS; i++){
        if(IRQ_P_IS_PENDING(interrupt_regs, i) && handlers[i] != 0){
            clearers[i]();
            handlers[i]();
            ENABLE_INTERRUPTS();
            return;
        }
    }
}
//register like arm timer interrupt
static void local_timer_handler(void)
{
    uint32_t value;
    uint32_t memory = *(volatile uint32_t*)CORE0_L_INT_SRC;
    uart_puts("Receive a generic timer interrupt ");
    uart_puts("Value of core0 int source: ");
    asm volatile("ldr %0, [%1]" : "=r"(value) : "r"(memory));
    value &= ~(0x8);
    uart_hex_puts(value);
}

static void local_timer_clearer(void)
{
    asm volatile("mrc p15, #0, r0, c14, c0, #0");//we obtain CNTFRQ
    asm volatile("mcr p15, #0, r0, c14, c3, #0");//1 irq per second
}
/**
 * this function is going to be called by the processor.  Needs to check pending interrupts and execute handlers if one is registered
 */

void __attribute__ ((interrupt ("ABORT"))) prefetch_abort_c_handler(void) {
    uart_puts("PREFETCH_ABORT Catched!\r\n");
}
void __attribute__ ((interrupt ("ABORT"))) data_abort_c_handler(void) {
    uart_puts("DATA_ABORT Catched!\r\n");
}
void __attribute__ ((interrupt ("ABORT"))) reset_c_handler(void) {
    uart_puts("RESET Catched!\r\n");
}
void __attribute__ ((interrupt ("UNDEF"))) undefined_instruction_c_handler(void) {
    uint32_t reg0;
    asm volatile("mov %0, r0":"=r" (reg0) :);
    uart_hex_puts(reg0);
    uart_puts("UNDEFINED_INSTRUCTION Catched!\r\n");
}
void __attribute__ ((interrupt ("SWI"))) software_interrupt_c_handler(void) {
    uart_puts("SWI Catched!\r\n");
}
void __attribute__ ((interrupt ("FIQ"))) fast_irq_c_handler(void) {
    uart_puts("FIQ Catched!\r\n");
}
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

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

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{   
    (void) r0;
    (void) r1;
    (void) atags;
    // HDMI
    mem_init(((atag_t *)atags));
    gpu_init();
    // Declare as unused
    
    
    /*uart_init();
    register_irq_handler(ARM_TIMER, local_timer_handler, local_timer_clearer);
    local_timer_init();
    interrupts_init();

    uart_hex_puts(r0);
    uart_puts("Hello, World!\r\n");*/
    /*
    //just for testing asm function
    uart_puts("CPSR.MODE[4:0] value is: ");
    uint32_t value = 1;
    uint32_t salida;
    asm volatile("mrs r0, cpsr");
    asm volatile("and r0, r0, #0x1f");
    asm volatile("mov %0, r0": "=r"(salida));
    uart_hex_puts(salida);
    */
    /*
    if (INTERRUPTS_ENABLED()){
        uart_puts("poz zi crack bro\r\n");
    }
    char c;
    //char buf[20];
    //unsigned int i;
    while (1) {
        uart_putc('>');
        while ((c = uart_getc()) != '\r' ){
            uart_putc(c);
        }
        uart_putc('\n');
    }
    */
    while (1) {
        io_halt();
    }
}
