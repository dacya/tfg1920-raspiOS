/**
* This code belongs to pcb.h and pcb.c and are a fundamental piece to the
* correct execution of that module. Edit with care.
*
* Alejandro Cancelo Correia
* March, 31, 2020
*/
.equ IRQ_STACK, 0x4000
.equ IRQ_MODE, 0x12
.equ SUPERVISOR_MODE, 0x13

.section ".text"

.extern __stack_memory
.extern __scheduler_finished
.extern __process_lr
.extern __process_sp
.extern uart_putln

pointer_test_subroutine:
    mov r0, r0
    push {lr}
    ldr r0, =text2
    bl uart_putln
    ldr r0, =text2
    bl uart_putln
    pop {lr}
    b .

.global pointer_test
/* r0 pointer r1 pointer */
pointer_test:
    push {lr}
    ldr r0, =text
    bl uart_putln
    ldr r0, =text
    bl uart_putln
    ldr r0, =text
    bl uart_putln
    ldr r0, =text
    bl uart_putln

    bl pointer_test_subroutine
    pop {lr}
    mov pc, lr

.global switch_process_context

/**
 * switch_process_context(proc_saved_state_t* old, proc_saved_state_t* new);
 * 
 * Switch two process context. Store into old thread's stack his processor state
 * and extract from new thread's stack the new processor state.
 *
 * @param old A pointer to old process control block
 * @param new A pointer pointer to new process control block
 */
switch_process_context:
/* IN THIS PART OF CODE WE WILL STORE PROCESS STATE OF THE FIRST ARGUMENT */
    /*
    IRQ STACK FROM __stack_memory WHEN ENTER THIS FUNCTION
    r0 < __stack_memory
    r1
    r2
    ...
    r12
    lr(irq register) pc (variable of pcb.h)
     */
    push {r4-r12} //this is necessary to accomplish the arm calling standard
    push {r0}
    ldr r0, [r0] //we load old process's stack pointer
    ldr r2, =__stack_memory
    ldr r2, [r2] // irq stack
    ldmfd r2!, {r3-r12} //load from r0 to r9 from irq stack
    stmfd r0!, {r3-r12} //we store into first argument stack
    ldmfd r2!, {r3-r6}  //we extract remaining registes r10-r12 and lr
    
    mov r7, r6
    mov r6, r3
    mov r3, r7

    mov r7, r5
    mov r5, r4
    mov r4, r7

    sub r3, r3, #0x4

    stmfd r0!, {r3-r6} //we store into first argument stack
    mrs r12, spsr //we obtain the corresponding value of CPSR for our old process

    //we change to supervisor to obtain the correct value of lr
    cps #SUPERVISOR_MODE 
    stmfd r0!, {lr}
    cps #IRQ_MODE //change to IRQ mode

    stmfd r0!, {r12}
    pop {r3} //we obtain old process's pcb pointer
    str r0, [r3] //we update old process's stack pointer
    /*
    OLD PROCESS STACK
    spsr (Saved Process State Register)
    lr(supervisor register) lr (variable of pcb.h)
    lr(irq register) pc (variable of pcb.h)
    r12
    r11
    r10
    r0
    ...
    r9
    */

/* IN THIS PART OF CODE WE WILL LOAD PROCESS STATE OF THE SECOND ARGUMENT*/
    ldr r0, [r1] // we load new process's stack pointer
    ldmfd r0!, {r3} //we load new process's spsr register 
    msr spsr_cxsf, r3 //we store new process's spsr register

    ldmfd r0!, {r3} //we obtain lr (variable of pcb.h)
    ldr r4, =__process_lr
    str r3, [r4]
    
    /* we load lr/pc(variable of pcb.h), r12, r11, r10 from new process's stack but
       because of the way ldmfd(pop), stmfd(push)operations works
       it will load into the lowest register in the list the
       first value pointed by r1(sp) which, in this case is lr*/
    ldmfd r0!, {r3, r4, r5, r6}//r3=lr, r4=r12, r5=r11, r6=r10
    //we turn the register list
    mov r7, r6
    mov r6, r3
    mov r3, r7

    mov r7, r5
    mov r5, r4
    mov r4, r7
    //r3=INIT(r10), r4=INIT(r11), r5=INIT(r12), r6=INIT(lr)
    stmfd r2!, {r3, r4, r5, r6}
    ldmfd r0!, {r3-r12} //we load r0 to r9 registers from INIT process's stack
    //r3=INIT(r0), r4=INIT(r1), r5=INIT(r2), r6=INIT(r3), r7=INIT(r4), r8=INIT(r5), r9=INIT(r6), r10=INIT(r7), r11=INIT(r8), r12=INIT(r9)
    stmfd r2!, {r3-r12}
    str r0, [r1] //we update new process's stack pointer
    ldr r2, =__scheduler_finished
    mov r1, #0x1
    str r1, [r2]

    ldr r1, =__process_sp //we store the stack pointer to retrieve it later
    str r0, [r1]
    /*
    IRQ STACK FROM __stack_memory WHEN FINISH THIS PART OF CODE
    r0(new process) < __stack_memory
    r1(new process)
    r2(new process)
    ...
    r12(new process)
    lr(new process)
    */
    pop {r4-r12}
    mov pc, lr


.global load_process
/**
 * load_init_process(proc_saved_state_t* process_pcb);
 * 
 * loads the next process's context.
 *
 * @param process_pcb A pointer to the next process control block
 */
load_process:
/*
    IRQ STACK FROM __stack_memory WHEN ENTER THIS FUNCTION
    r0 < __stack_memory
    r1
    r2
    ...
    r12
    lr

    IRQ FROM __stack_memory + 0x38
    r0
    r1
    r2
    ...
    r12
    lr
    x < r2
 */
    push {r4-r7}
    ldr r2, =__stack_memory
    ldr r2, [r2] // irq stack
    add r2, r2, #0x38
    mov r1, r0
    ldr r0, [r1] // we load process's stack pointer
    ldmfd r0!, {r3} //we load process's spsr register 
    msr spsr_cxsf, r3 //we store new process's spsr register

    ldmfd r0!, {r3} //we obtain lr (variable of pcb.h)
    ldr r4, =__process_lr
    str r3, [r4]

    /* we load lr/pc(variable of pcb.h), r12, r11, r10 from process's stack but
       because of the way ldmfd(pop), stmfd(push)operations works
       it will load into the lowest register in the list the
       first value pointed by r0(sp) which, in this case is lr*/
    ldmfd r0!, {r3, r4, r5, r6} //r3=lr/pc, r4=r12, r5=r11, r6=r10
    //we turn the register list
    mov r7, r6
    mov r6, r3
    mov r3, r7

    mov r7, r5
    mov r5, r4
    mov r4, r7
    //r3=PROCESS(r10), r4=PROCESS(r11), r5=PROCESS(r12), r6=PROCESS(lr)
    stmfd r2!, {r3, r4, r5, r6} 
    ldmfd r0!, {r3-r12} //we load r0 to r9 registers from INIT process's stack
    //r3=PROCESS(r0), r4=PROCESS(r1), r5=PROCESS(r2), r6=PROCESS(r3), r7=PROCESS(r4), r8=PROCESS(r5),
    //r9=PROCESS(r6), r10=PROCESS(r7), r11=PROCESS(r8), r12=PROCESS(r9)
    stmfd r2!, {r3-r12}
    str r0, [r1] //we update INIT process's stack pointer
    ldr r2, =__scheduler_finished
    mov r1, #0x1
    str r1, [r2]

    ldr r1, =__process_sp//we store the stack pointer to retrieve it later
    str r0, [r1]

    pop {r4-r7}
    mov pc, lr


.global yield_to_next_process
/**
 * yield_to_next_process(proc_saved_state_t* process_pcb);
 * 
 * This function will implement the same context switch than load_process()
 * but it will be called by the reap function instead of been invoked by the 
 * scheduler.
 *
 * To all means, this function will change all the registers from the last
 * process to all register belonging to process_pcb argument and return execution
 * to where process_pcb was interrupted.
 *
 * @param process_pcb A pointer to init process control block
 * @see load_process()
 * @see reap()
 */
yield_to_next_process:
/* REMEMBER: the state of process_pcb's stack is like the following
    cpsr //(Saved Process State Register)
    lr //pointer to return address
    pc //pointer to the corresponding thread
    r12
    r11
    r10
    r0
    r1
    r2
    r3
    r4
    r5 
    r6
    r7 < point E
    r8 
    r9 */
    mov sp, #IRQ_STACK
    mov r7, r0 //direction to pcb
    ldr r8, [r0] //we load the stack pointer of process_pcb argument
    ldmfd r8!, {r1} //we load cpsr
    msr cpsr_cxsf, r1 //we do the following assingment cpsr = r1
    ldmfd r8!, {lr} //we load lr
    ldmfd r8!, {r9} //we load pc in r9
    /* we load r12, r11, r10 from process's stack but
       because of the way ldmfd(pop), stmfd(push)operations works
       it will load into the lowest register in the list the
       first value pointed by r8(sp) which, in this case is r12*/
    ldmfd r8!, {r10, r11, r12}
    mov r7, r10
    mov r10, r12
    mov r12, r7
    ldmfd r8!, {r0-r5}
    add r6, r8, #0x10
    str r6, [r7] //we update process's stack
    mov sp, r6
    ldmfd r8!, {r6} // point E
    ldr r7, [r8, #0x8]
    str r9, [r8, #0x8]
    mov r9, r7
    ldmfd r8, {r7, r8, pc}

.data
.balign 4
text: .asciz "test_yes"
text2: .asciz "yes"