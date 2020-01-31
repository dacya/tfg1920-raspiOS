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

//TODO: try to read how to adapt it for local IRQ

#include <stdint.h>
#include <io/base.h>

/**
 * Peripheral base memory address for controlling interrupts
 * Documented in BCM2837/5 ARM Peripheral
*/
#define INTERRUPTS_P_BASE (PHYSICAL_PBASE + 0x0000B000)
/**
 * Peripheral pending memory address for interrupts
 * Documented in BCM2837/5 ARM Peripheral
*/
#define INTERRUPTS_P_PENDING (INTERRUPTS_P_BASE + 0x200)
/**
 * Memory base address for local interrupts (refering as local to the cores)
 * Documented in BCM2836 ARM-local peripherals
*/
#define INTERRUPTS_L_BASE (0x40000000)
/**Address: 0x4000_0040 Core 0 Timers interrupt control
Address: 0x4000_0044 Core 1 Timers interrupt control
Address: 0x4000_0048 Core 2 Timers interrupt control
Address: 0x4000_004C Core 3 Timers interrupt control*/
#define CORE0_L_TIMER_INT_CTL (INTERRUPTS_L_BASE + 0x40)
#define CORE1_L_TIMER_INT_CTL (INTERRUPTS_L_BASE + 0x44)
#define CORE2_L_TIMER_INT_CTL (INTERRUPTS_L_BASE + 0x48)
#define CORE3_L_TIMER_INT_CTL (INTERRUPTS_L_BASE + 0x4c)

/**
 * Address: 0x4000_0060 Core0 interrupt source
Address: 0x4000_0064 Core1 interrupt source
Address: 0x4000_0068 Core2 interrupt source
Address: 0x4000_006C Core3 interrupt source
*/
#define CORE0_L_INT_SRC (INTERRUPTS_L_BASE + 0x60)
#define CORE1_L_INT_SRC (INTERRUPTS_L_BASE + 0x64)
#define CORE2_L_INT_SRC (INTERRUPTS_L_BASE + 0x68)
#define CORE3_L_INT_SRC (INTERRUPTS_L_BASE + 0x6c)

#define NUM_P_IRQS 72
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
#define IRQ_P_IS_BASIC(x) ((x >= 64 ) && (x < NUM_P_IRQS))
/**
 * Better definition about this in "ARM interrupt table" from section
 * 7.5 of BCM2837/6/5 peripherals documentation and below
*/
#define IRQ_P_IS_GPU2(x) ((x >= 32 && x < 64 ))
/**
 * Better definition about this in "ARM interrupt table" from section
 * 7.5 of BCM2837/6/5 peripherals documentation and below
*/
#define IRQ_P_IS_GPU1(x) ((x < 32 ))
/**
 * @param (interrupt_registers_t)regs (irq_number_t) num 
 * This Macro helps to determine whether the num irq is pending or not
 * @returns True if it is pending or False if it isn't
*/
#define IRQ_P_IS_PENDING(regs, num) \
  ((IRQ_P_IS_BASIC(num) && ((1 << (num-64)) & regs->irq_base_int_pending)) || \
  (IRQ_P_IS_GPU2(num) && ((1 << (num-32)) & regs->irq_pending2))       || \
  (IRQ_P_IS_GPU1(num) && ((1 << (num)) & regs->irq_pending1)))


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
 * Help us control, write to and read from the peripheral interrupt registers
 * defined in section 7. of BCM2837/5 Peripherals manual.
 * It must be used as a pointer for a correct manage.
*/
typedef struct {
    /**
     * Register for these types of peripheral interrupts 
     * ARM Timer
     * ARM Mailbox
     * ARM Doorbell 0
     * ARM Doorbell 1
     * GPU0 halted (Or GPU1 if bit 10 of control register 1 is set)
     * GPU1 halted
     * Illegal access type 0
     * Illegal access type 1
    */
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