.equ VECTOR_TABLE_INIT, 0x00000000
.equ IRQ_MODE, 0x12
.equ FIQ_MODE, 0x11
.equ SUPERVISOR_MODE, 0x13
.equ ABORT_MODE, 0x17
.equ UNDEFINED_MODE, 0x1B
.equ SYSTEM_MODE, 0x1F
/* 
included but not used
.equ HYPERVISOR_MODE, 0x1A
.equ MONITOR_MODE, 0x15 */

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
    push    {r4-r9, lr}
    ldr     r0, =vector_table 
    mov     r1, #VECTOR_TABLE_INIT
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    //These lines are necessary to keep the references of #offset from
    //ldr pc, [pc + #offset] relatives
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8}
    //-----
    pop     {r4-r9, lr}
    mov pc, lr //copies the addres of the next instruction


halt:
    //wfe //wait for event
    wfi
    b halt


undefined_instruction_s_handler:
    //nothing to adjust (Cortex-A PG)
    srsdb sp!, #UNDEFINED_MODE
    /*
    It is necessary to switch to supervisor mode and store some registers
    into it's stack for having support for nested exceptions
     */
    bl undefined_instruction_c_handler
    //I need to something more here
    rfeia sp! //we do the inverse operation of srsdb


software_interrupt_s_handler:
    //nothing to adjust (Cortex-A PG)
    srsdb sp!, #SUPERVISOR_MODE //Supervisor mode
    bl software_interrupt_c_handler
    //I need to something more here
    rfeia sp! //we do the inverse operation of srsdb


prefetch_abort_s_handler:
    sub lr, lr, #4 //we adjust the appropiate value considered by Cortex-A PG
    srsdb sp!, #ABORT_MODE //abort mode
    /*
    It is necessary to switch to supervisor mode and store some registers
    into it's stack for having support for nested exceptions
     */
    bl prefetch_abort_c_handler
    //I need to something more here
    rfeia sp! //we do the inverse operation of srsdb


data_abort_s_handler:
    sub lr, lr, #8 //we adjust the appropiate value considered by Cortex-A PG
    srsdb sp!, #ABORT_MODE //abort mode
    /*
    It is necessary to switch to supervisor mode and store some registers
    into it's stack for having support for nested exceptions
     */
    bl data_abort_c_handler
    rfeia sp! //we do the inverse operation of srsdb

//Simple version with no nested interrupts support
.extern __stack_memory
.extern __reap_pointer
.extern __scheduler_finished
irq_s_handler:
    //srsdb sp!, #IRQ_MODE //we stores the spsr and lr at the address contained in sp of the mode irq
    /*
    It is necessary to switch to supervisor mode and store some registers
    into it's stack for having support for nested exceptions
    */
    push {r0-r12, lr}
    ldr r12, =__stack_memory
    str sp, [r12]
    ldr r0, =__scheduler_finished
    mov r12, #0x0
    str r12, [r0]
    bl irq_c_handler
    pop {r0-r12, lr}

    push {r0}
    ldr r0, =__scheduler_finished 
    ldr r0, [r0]
    cmp r0, #0x1 //Have the scheduler interrupt been treated?
    bne normal_irq_execution
        pop {r0} //we recover the correct value of r0
        push {r0, lr} //we store r0 with lr into irq stack
        ldr r0, =__reap_pointer
        ldr r0, [r0] //we load where does the reap function start
        mov lr, r0 //we make sure lr contains a pointer to reap

        ldr r0, =__stack_memory
        str sp, [r0]

        mrs r0, spsr
        msr cpsr_cxsf, r0 //we change cpsr to the corresponding value of cpsr of the new process
        /* we load the correct value of r0 and we return to where our new
           process were interrupted */
           //we change execution mode so does the stack
        
        ldr r0, =__stack_memory
        ldr r0, [r0]
        ldmfd r0, {r0, pc}

normal_irq_execution:
    pop {r0}
    //rfeia sp! //we do the inverse operation of srsdb
    subs pc, lr, #4

fast_irq_s_handler:
    sub lr, lr, #4 //we adjust the appropiate value considered by Cortex-A PG
    srsdb sp!, #FIQ_MODE //fiq mode
    /*
    It is necessary to switch to supervisor mode and store some registers
    into it's stack for having support for nested exceptions
     */
    bl fast_irq_c_handler
    //I need to something more here
    rfeia sp! //we do the inverse operation of srsdb