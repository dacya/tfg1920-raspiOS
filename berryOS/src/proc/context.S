.section ".text"

.global switch_to_thread

/**
 * switch_to_thread(process_control_block_t * old, process_control_block_t * new);
 *
 *Swith two threads, storing in the stack the old thread registers
 *and extracting those of the new thread.
 *
 * @param old The context to store
 * @param new The context to extract
 */
switch_to_thread:
    push {lr}
    push {sp}
    mrs r12, cpsr
    push {r0-r12}
    str sp, [r0]
    ldr sp, [r1]
    //ldr r0, =#10000
    //bl timer_set
    pop {r0-r12}
    msr cpsr_c, r12
    pop {lr, pc}