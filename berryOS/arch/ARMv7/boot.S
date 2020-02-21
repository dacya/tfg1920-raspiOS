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
    //Just for pi2 and pi3!
    mrc p15, #0, r1, c0, c0, #5 
    and r1, r1, #3 //it takes the last two bits
    cmp r1, #0
    bne halt

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

    cpsid i
    /* INITIALIZE EXCEPTION VECTORS */

    ldr r3, =str_vector_table
    blx r3
    /* INITIALIZE THE MEMORY SYSTEM, INCLUDIN MMU */
    //We won't use the MMU, only dynamic memory
    //code here...

    /* INITIALIZE CORE MODE STACKS AND REGISTERS */
    //We start execution in Supervisor (SVC) mode
    //change to IRQ mode
    cps #0x12 //change to IRQ mode
    mov sp, #0x4000 
    //we change again to svc mode
    cps #0x13 
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
    // Call kernel_main
    mov r5, pc
    ldr r3, =kernel_main
    blx r3

halt:
    //wfe //wait for event
    wfi
    b halt

.globl io_halt
io_halt:
    wfi
    bx lr


.globl _local_timer_init
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