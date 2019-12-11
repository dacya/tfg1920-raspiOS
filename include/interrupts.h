/**
 * This is a simple library for controlling and defining exception handlers
 * in a simple way. 
 * 
 * Alejandro Cancelo Correia
 * 
 * Nov, 10, 2019
*/
#ifndef _INT_HANDLING_H
#define _INT_HANDLING_H

#include <stdint.h>
#include <peripherals/base.h>

#define INTERRUPTS_BASE (PHYSICAL_PBASE + 0x0000B000)
#define INTERRUPTS_PENDING (INTERRUPTS_BASE + 0x200)

#define NUM_IRQS 72
/**
 * Macro for distinguis if the irq is one of the following:
 * ARM Timer
 * ARM Mailbox
 * ARM Doorbell 0
 * ARM Doorbell 1
 * GPU0 halted (Or GPU1 if bit 10 of control register 1 is set)
 * GPU1 halted
 * Illegal access type 0
 * Illegal access type 1
*/
#define IRQ_IS_BASIC(x) ((x >= 64 ) && (x < NUM_IRQS))
/**
 * Better definition about this in "ARM interrupt table" from section
 * 7.5 of BCM2837/6/5 peripherals documentation and below
*/
#define IRQ_IS_GPU2(x) ((x >= 32 && x < 64 ))
/**
 * Better definition about this in "ARM interrupt table" from section
 * 7.5 of BCM2837/6/5 peripherals documentation and below
*/
#define IRQ_IS_GPU1(x) ((x < 32 ))
/**
 * @param (interrupt_registers_t)regs (irq_number_t) num 
 * This Macro helps to determine whether the num irq is pending or not
 * @returns True if it is pending or False if it isn't
*/
#define IRQ_IS_PENDING(regs, num) \
  ((IRQ_IS_BASIC(num) && ((1 << (num-64)) & regs->irq_basic_pending)) || \
  (IRQ_IS_GPU2(num) && ((1 << (num-32)) & regs->irq_gpu_pending2))    || \
  (IRQ_IS_GPU1(num) && ((1 << (num)) & regs->irq_gpu_pending1)))


/**
 * Pointer to a handler function
*/
typedef void (*interrupt_handler_f_ptr)(void);
/**
 * Pointer to a clear interrupt function
*/
typedef void (*interrupt_clearer_f_ptr)(void);


typedef enum {
    SYSTEM_TIMER_1 = 1,
    USB_CONTROLER = 9,
    ARM_TIMER = 64
} irq_number_t;

/**
 * Help us control, write to and read from the interrupt registers
 * defined in section 7. of BCM2837/6/5 Peripherals manual.
 * It must be used as a pointer for a correct manage.
*/
typedef struct {
    uint32_t irq_base_int_pending;
    //some entries might be wired to a GPU interrupt source 
    uint32_t irq_pending1; 
    //some entries might be wired to a GPU interrupt source
    uint32_t irq_pending2; 
    uint32_t fiq_control;
    //some entries might be wired to a GPU interrupt source 
    uint32_t irq_enable1; 
    //some entries might be wired to a GPU interrupt source
    uint32_t irq_enable2; 
    uint32_t irq_base_int_enable;
    //it has the same property as irq_gpu_enable1
    uint32_t irq_disable1; 
    //it has the same property as irq_gpu_enable2
    uint32_t irq_disable2; 
    uint32_t irq_base_int_disable;
} interrupt_registers_t;

/**
 * @param void
 * -----------------------------------
 * This will initialize interrupts creating the corresponding vector table 
 * and set up the enabling HW bit
 * -----------------------------------
 * @return (void)
*/
void interrupts_init(void);
/**
 * @param irq_num 
 * Type of interrupt
 * @param handler_to_register 
 * Function pointer to a ISR
 * @param clearer_to_register
 * Function pointer to clearer for ISR
 * ------------------------------------
 * This function will associate a function handler and a function clearer
 * to the type of interrupt irq_num.
 * ------------------------------------
 * @return void
*/
void register_irq_handler(irq_number_t irq_num, 
                          interrupt_handler_f_ptr handler_to_register,
                          interrupt_clearer_f_ptr clearer_to_register);
/**
 * @param irq_num
 * Type of interrupt 
 * -----------------------------------
 * This function will remove the function handler and clearer associate to
 * irq_num
 * -----------------------------------
 * @return void
*/
void unregister_irq_isr(irq_number_t irq_num);

/**
 * Macro to determine if the interrupts are enable in the hardware
 * -----------------------------------
 * @return True if enabled False if not
*/
__inline__ int INTERRUPTS_ENABLED(void) {
    int res;
    __asm__ __volatile__("mrs %[res], CPSR": [res] "=r" (res)::);
    return ((res >> 7) & 1) == 0;
}

/**
 * Macro to enable interrupts in the hardware.
 * It does nothing if interrupts are already enabled
 * -----------------------------------
 * @return void
*/
__inline__ void ENABLE_INTERRUPTS(void) {
    if (!INTERRUPTS_ENABLED()) {
        __asm__ __volatile__("cpsie i");
    }
}

/**
 * Macro to disable interrupts in the hardware
 * It does nothing if interrupts are already disable
 * -----------------------------------
 * @return void
*/
__inline__ void DISABLE_INTERRUPTS(void) {
    if (INTERRUPTS_ENABLED()) {
        __asm__ __volatile__("cpsid i");
    }
}

#endif