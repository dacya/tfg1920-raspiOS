// To keep this in the first portion of the binary.
.section ".text.boot"

// Make _start global.
.globl _start
/* 
                Entry point for the kernel.
  r15 -> should begin execution at 0x8000(32K). [r15 is PC register]
  r0 -> 0x00000000
  r1 -> 0x00000C42
  r2 -> 0x00000100 - start of ATAGS (parameters for the kernel)
  preserve these registers as argument for kernel_main
*/

/*
                Booting process
  1-> Put non-primary cores to sleep
  2-> Initialize exception vectors
  3-> Initialize the memory system, including MMU
  4-> Initialize core mode stacks and registers
  5-> Initialize any critical I/O devices
  6-> Perform any necessary initialization of NEON or VFP
  7-> Enable interrupts
  8-> Change core mode or state
  9-> Handle any set up require for Secure world
 10-> Call the main() application
*/
_start:
    /* NON-PRIMARY CORES PUTTED TO SLEEP */

// Model 1 only has 1 cpu and does not have this instruction, so don't include it if building for model 1
#ifndef MODEL_1
    mrc p15, #0, r1, c0, c0, #5 
    and r1, r1, #3 //it takes the last two bits
    cmp r1, #0
    bne halt
#endif

    /* INITIALIZE EXCEPTION VECTORS */

    ldr r3, =str_vector_table
    blx r3
    /* INITIALIZE THE MEMORY SYSTEM, INCLUDIN MMU */
    //We won't use the MMU, only dynamic memory
    //code here...

    /* INITIALIZE CORE MODE STACKS AND REGISTERS */
    //We start execution in Supervisor (SVC) mode
    mrs r0, cpsr
    //change to IRQ mode
    bic r1, r0, #0x1f //clear mode bits
    orr r1, r0, #0x12 //irq mode set
    msr cpsr_c, r1
    mov sp, #0x4000 
    //we change again to svc mode
    msr cpsr, r0
    mov sp, #0x8000

    /* INITIALIZE ANY CRITICAL I/O DEVICES */
    //Code here...

    /* PERFORM ANY NECESSARY INITIALIZATION OF NEON OR VFP */
    //we won't use the Neon or VFP hardware
    //Code here...

    /* ENABLE INTERRUPTS */
    cpsie i

    /* CHANGE CORE MODE OR STATE */
    //Because the kernel runs un Supervisor mode, this step is done at the end
    //of the step 4

    /* HANDLE ANY SET UP REQUIRE FOR SECURE WORLD */
    //we won't use the Secure support
    //Code here...

    /* CALL THE MAIN() APPLICATION */
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

    // Call kernel_main
    mrs r0, cpsr
    ldr r3, =kernel_main
    blx r3

// halt
halt:
    wfe //wait for event
    //wfi
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
    /* Also called Supervisor call
       Mode: PL1 svc 
    */
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
    /*Mode: PL1 abt */
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
    /*Mode: PL1 abt */
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
    /*Mode: PL1 irq */
    //srsdb sp!, #0x12 //we stores the spsr and lr at the address contained in sp of the mode irq
    /*
    It is necessary to switch to supervisor mode and store some registers
    into it's stack for having support for nested exceptions
     */
    push {r0-r12, lr}
    bl irq_c_handler
    pop {r0-r12, lr}
    //rfeia sp! //we do the inverse operation of srsdb
    sub pc, lr, #4 //we adjust the appropiate value considered by Cortex-A PG



fast_irq_s_handler:
    /*Mode: PL1 fiq */
    sub lr, lr, #4 //we adjust the appropiate value considered by Cortex-A PG
    srsdb sp!, #0x11 //fiq mode
    /*
    It is necessary to switch to supervisor mode and store some registers
    into it's stack for having support for nested exceptions
     */
    bl fast_irq_c_handler
    //I need to something more here
    rfeia sp! //we do the inverse operation of srsdb

.globl io_halt
io_halt:
    wfi
    bx lr


.globl _local_timer_init
/*
        THIS STEPS APPLIES IN A SYSTEM WHERE THERE IS NOT VIRTUALIZATION SUPPORT

    1. Look into CNTKCTL register if you need
    2. Look into CNTP_CTL or CNTH_CTL or CNTV_CTL to enable or disable
       the corresponding timer (bit 0)
    3. You have to set the compare value for the corresponding timer
       CNTP_CVAL, CNTH_CVAL, CNTV_CVAL if needed
    4. It should be in boot.S but you have to initialize the counter 
       frequency register, CNTFRQ
    5. Putting the corresponding TVAL register to a right value
    6. Routing the IRQ and enabling IRQ of the corresponding core
*/
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