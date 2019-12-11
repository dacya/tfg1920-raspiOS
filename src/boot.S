// To keep this in the first portion of the binary.
.section ".text.boot"

// Make _start global.
.globl _start
// Entry point for the kernel.
// r15 -> should begin execution at 0x8000(32K). [r15 is PC register]
// r0 -> 0x00000000
// r1 -> 0x00000C42
// r2 -> 0x00000100 - start of ATAGS (parameters for the kernel)
// preserve these registers as argument for kernel_main
_start:
    // This set of instructions sets 3 of the 4 cores to go to halt.
    // Model 1 only has 1 cpu and does not have this instruction, so don't include it if building for model 1
#ifndef MODEL_1
    /*
      mrc pn (coprocessor register) op1 Rd CRn CRm op2
      the functionality is define in section 4.2.1 of Cortex-A7 MPcore technical reference manual(TM).
      To understand the instruction you have to also read section 3.1.9 in page 3-9 of 
      Cortex-A Series Programmer's guide(PG)
    */
    //it reads the Multiprocessor Affinity Register and stores it value it r1.
    //it is defined in page 4-28 of (TM)
    mrc p15, #0, r1, c0, c0, #5 
    and r1, r1, #3 //it takes the last two bits
    cmp r1, #0
    bne halt
#endif
    // Setup the stack.
    mov sp, #0x8000

    // Clear out bss.
    ldr r4, =__bss_start
    ldr r9, =__bss_end
    mov r5, #0
    mov r6, #0
    mov r7, #0
    mov r8, #0
    b       2f

1:
    // store multiple at r4.
    stmia r4!, {r5-r8}

// If we are still below bss_end, loop.
2:
    cmp r4, r9
    blo 1b

    // create the vector table
    ldr r3, =str_vector_table
    blx r3
    //enable interrupts
    cpsie i //i'm not sure it is necessary
    /*
    ldr r0, =undefined_instruction_exception_addr
    //mrs r0, CPSR
    //to debug only
    */
    // Call kernel_main
    ldr r3, =kernel_main
    blx r3

// halt
halt:
    wfe
    b halt

//we define the vector table and we will be using ldr instrunctions because a
//branch instruction have a limited range of 32 MB and our c handler routine
//may be far from that
vector_table:
    ldr pc, reset_exception_addr 
    ldr pc, undefined_instruction_exception_addr
    ldr pc, software_interrupt_exception_addr
    ldr pc, prefetch_abort_exception_addr
    ldr pc, data_abort_exception_addr
    nop // This one is reserved for hypervisor mode
    ldr pc, irq_handler_abs_addr
    ldr pc, fast_irq_handler_abs_addr


reset_exception_addr:                 .word reset_c_handler 
undefined_instruction_exception_addr: .word undefined_instruction_c_handler
software_interrupt_exception_addr:    .word software_interrupt_c_handler
prefetch_abort_exception_addr:        .word prefetch_abort_c_handler
data_abort_exception_addr:            .word data_abort_c_handler
irq_handler_abs_addr:                 .word irq_s_handler
fast_irq_handler_abs_addr:            .word fast_irq_c_handler

// asm function to store the vector table into the default memory area
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
    sub lr, lr, #4 //we adjust the appropiate value considered by Cortex-A PG
    srsdb sp!, #0x12 //we stores the spsr and lr at the address contained in sp of the mode irq
    /*
    It is necessary to switch to supervisor mode and store some registers
    into it's stack for having support for nested exceptions
     */
    bl irq_c_handler
    rfeia sp! //we do the inverse operation of srsdb


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