.equ locked, 1
.equ unlocked, 0

//lock_mutex
//Declare for use from C as extern void lock_mutex(void * mutex);
.global lock_mutex
/**
 * It will try to take the mutex passed as an argument. If it fails, it will
 * go to power saving mode until the scheduler change the process.
 *
 * What to do when it fails can be change over time, there are two efficient
 * possibilities:
 * 1 - As metion above, one is put the processor in power saving mode with the
 *    help of WFI and WFE/SEV instructions (See the architectural manual for
 *    more information about it)
 * 2 - Request to OS to schedule in a new process and try to take the lock later
 *
 * @param mutex The mutex needed to perform an attempt to obtain it, it MUST BE
 * initialized to one unlocked first
 */
.extern uart_putln
lock_mutex:
    ldr r1, =locked
test_lock:
    ldrex r2, [r0]
    cmp r2, r1 // Test if mutex is locked or unlocked
    beq wait_lock   // If locked - wait for it to be released, from 2
        strexne r2, r1, [r0] // Not locked, attempt to lock it
        cmpne r2, #1 // Check if Store-Exclusive failed
        beq test_lock // Failed - retry from 1
            // Lock acquired
            dmb //Required before accessing protected resource
            mov pc, lr
wait_lock: 
    //WAIT_FOR_UPDATE //Take appropriate action while waiting for mutex to become unlocked
    wfi
    nop //its here but it will never be executed
    b test_lock  //Retry from 1


// unlock_mutex
// Declare for use from C as extern void unlock_mutex(void * mutex)//
.global unlock_mutex
unlock_mutex:
    ldr r1, =unlocked
    dmb // Required before releasing protected resource
    str r1, [r0] // Unlock mutex
    //SIGNAL_UPDATE
    mov pc, lr