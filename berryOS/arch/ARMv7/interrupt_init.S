
//we define the vector table and we will be using ldr instrunctions because a
//branch instruction have a limited range of 32 MB and our c handler routine
//may be far from that
vector_table:
    ldr pc, reset_exception_addr 
    ldr pc, undefined_instruction_exception_addr
    ldr pc, software_interrupt_exception_addr
    ldr pc, prefetch_abort_exception_addr
    ldr pc, data_abort_exception_addr
    ldr pc, reserved_exception_addr // This one is reserved for hypervisor mode
    ldr pc, irq_handler_abs_addr
    ldr pc, fast_irq_handler_abs_addr


reset_exception_addr:                 .word reset_c_handler 
undefined_instruction_exception_addr: .word undefined_instruction_c_handler
software_interrupt_exception_addr:    .word software_interrupt_c_handler
prefetch_abort_exception_addr:        .word prefetch_abort_c_handler
data_abort_exception_addr:            .word data_abort_c_handler
reserved_exception_addr:              .word halt
irq_handler_abs_addr:                 .word irq_s_handler
fast_irq_handler_abs_addr:            .word fast_irq_c_handler

// asm function to store the vector table into the default memory area
.globl str_vector_table
str_vector_table:
    push    {r4, r5, r6, r7, r8, r9}
    ldr     r0, =vector_table 
    mov     r1, #0x0000
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    //These lines are necessary to keep the references of #offset from
    //ldr pc, [pc + #offset] relatives
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8}
    //-----
    pop     {r4, r5, r6, r7, r8, r9}
    blx     lr //copies the addres of the next instruction


halt:
    //wfe //wait for event
    wfi
    b halt


undefined_instruction_s_handler:
    //nothing to adjust (Cortex-A PG)
    srsdb sp!, #0x1B
    /*
    It is necessary to switch to supervisor mode and store some registers
    into it's stack for having support for nested exceptions
     */
    bl undefined_instruction_c_handler
    //I need to something more here
    rfeia sp! //we do the inverse operation of srsdb


software_interrupt_s_handler:
    //nothing to adjust (Cortex-A PG)
    srsdb sp!, #0x13 //Supervisor mode
    bl software_interrupt_c_handler
    //I need to something more here
    rfeia sp! //we do the inverse operation of srsdb


prefetch_abort_s_handler:
    sub lr, lr, #4 //we adjust the appropiate value considered by Cortex-A PG
    srsdb sp!, #0x17 //abort mode
    /*
    It is necessary to switch to supervisor mode and store some registers
    into it's stack for having support for nested exceptions
     */
    bl prefetch_abort_c_handler
    //I need to something more here
    rfeia sp! //we do the inverse operation of srsdb


data_abort_s_handler:
    sub lr, lr, #8 //we adjust the appropiate value considered by Cortex-A PG
    srsdb sp!, #0x17 //abort mode
    /*
    It is necessary to switch to supervisor mode and store some registers
    into it's stack for having support for nested exceptions
     */
    bl data_abort_c_handler
    rfeia sp! //we do the inverse operation of srsdb

//Simple version with no nested interrupts support
irq_s_handler:
    //srsdb sp!, #0x12 //we stores the spsr and lr at the address contained in sp of the mode irq
    /*
    It is necessary to switch to supervisor mode and store some registers
    into it's stack for having support for nested exceptions
    */
    push {r0-r12, lr}
    bl irq_c_handler
    pop {r0-r12, lr}
    //rfeia sp! //we do the inverse operation of srsdb
    subs pc, lr, #4


fast_irq_s_handler:
    sub lr, lr, #4 //we adjust the appropiate value considered by Cortex-A PG
    srsdb sp!, #0x11 //fiq mode
    /*
    It is necessary to switch to supervisor mode and store some registers
    into it's stack for having support for nested exceptions
     */
    bl fast_irq_c_handler
    //I need to something more here
    rfeia sp! //we do the inverse operation of srsdb
