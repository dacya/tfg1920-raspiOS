#ifndef MUTEX_H
#define MUTEX_H

#define MUTEX_LOCK 1
#define MUTEX_UNLOCK 0

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
 * @param mutex A pointer to the mutex needed to perform an attempt to obtain it, 
 * it <b>MUST BE</b> initialized to unlocked first.
 */
void take_lock(void* mutex);

/**
 * It will free the lock passed as an argument. This function always works
 * 
 * @param mutex A pointer to the mutex needed to perform the free action
*/
void free_lock(void* mutex);


#endif // MUTEX_H
