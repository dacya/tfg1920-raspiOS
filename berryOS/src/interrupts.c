#include <interrupts.h>
#include <stddef.h>
#include <io/uart.h>
#include <local_timer.h>
#include <proc/pcb.h>
//from local timer
extern timer_selection_t core_timer_selected;
extern unsigned int current_time_value;

extern uint8_t __stack_memory;

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
    /*
    it should create the vector table and enable interrupts.
    We are doing it in boot.S
    */
}

void register_irq_handler(irq_number_t irq_num, interrupt_handler_f_ptr handler_to_register, interrupt_clearer_f_ptr clearer_to_register) {
    uint32_t pos;
    if(IRQ_P_IS_BASIC(irq_num)) {
        pos = irq_num - 64; //to get a number between 0 and 7
        handlers[irq_num] = handler_to_register;
        clearers[irq_num] = clearer_to_register;
        interrupt_regs->irq_base_int_disable &= ~(1 << pos);
        interrupt_regs->irq_base_int_enable |= (1 << pos);
    } else if (IRQ_P_IS_GPU2(irq_num)) {
        pos = irq_num - 32; // to get a number between 0 and 31
        handlers[irq_num] = handler_to_register;
        clearers[irq_num] = clearer_to_register;
        interrupt_regs->irq_disable2 &= ~(1 << pos);
        interrupt_regs->irq_enable2 |= (1 << pos);
    } else if (IRQ_P_IS_GPU1(irq_num)) {
        handlers[irq_num] = handler_to_register;
        clearers[irq_num] = clearer_to_register;
        interrupt_regs->irq_disable1 &= ~(1 << irq_num);
        interrupt_regs->irq_enable1 |= (1 << irq_num);
    } else {
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

void print_irq_stack(void){
    uint32_t* stack_memory = (uint32_t*)&__stack_memory;
    uart_putln("\n--------------PRINT_IRQ_STACK----------------------");
    uart_putln("stack_memory_tag is inside of:");
    uart_hex_puts((uint32_t)stack_memory);
    uint32_t* stack = (uint32_t*)(*stack_memory);
    uart_putln("registers:");
    for(unsigned int i = 0; i <= 13; i++ ){
        uart_puts("stack_memory: ");
        uart_hex_puts((uint32_t)(stack+i));
        uart_puts("value inside:");
        uart_hex_puts(*(stack+i));
        uart_putln("");
    }
    uart_putln("--------------PRINT_IRQ_STACK END----------------------");
}

//TODO: FIX THE SECOND LOOP LOGIC
void irq_c_handler(void) {
    if((*(volatile uint32_t*)CORE0_L_INT_SRC) & 0x8) {
        clearers[ARM_TIMER]();
        handlers[ARM_TIMER]();
    }
    ENABLE_INTERRUPTS();
    //the cpu reaches this function with the IRQ exceptions disabled
    /* 
    for(int i = 0; i < NUM_P_IRQS; i++){
        if(IRQ_P_IS_PENDING(interrupt_regs, i) && handlers[i] != 0){
            clearers[i]();
            handlers[i]();
            ENABLE_INTERRUPTS();
            return;
        }
    } */
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
    uart_puts("UNDEFINED_INSTRUCTION Catched!\r\n");
}
void __attribute__ ((interrupt ("SWI"))) software_interrupt_c_handler(void) {
    uart_puts("SWI Catched!\r\n");
}
void __attribute__ ((interrupt ("FIQ"))) fast_irq_c_handler(void) {
    uart_puts("FIQ Catched!\r\n");
}


int INTERRUPTS_ENABLED(void) {
    int res;
    __asm__ __volatile__("mrs %[res], CPSR": [res] "=r" (res)::);
    return ((res >> 7) & 1) == 0;
}

void ENABLE_INTERRUPTS(void) {
    if (!INTERRUPTS_ENABLED()) {
        __asm__ __volatile__("cpsie i");
    }
}

void DISABLE_INTERRUPTS(void) {
    if (INTERRUPTS_ENABLED()) {
        __asm__ __volatile__("cpsid i");
    }
}

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
