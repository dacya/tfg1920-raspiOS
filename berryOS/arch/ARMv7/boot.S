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

    ldr r4, =__bss_start
    ldr r9, =__bss_end
    mov r5, #0
    mov r6, #0
    mov r7, #0
    mov r8, #0
    b       2f

1:
    stmia r4!, {r5-r8}

2:
    cmp r4, r9
    blo 1b

    cpsid i
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
    /* INITIALIZE EXCEPTION VECTORS */

    bl str_vector_table
    
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
    bl kernel_main

halt:
    //wfe //wait for event
    wfi
    b halt

.globl io_halt
io_halt:
    wfi
    bx lr
