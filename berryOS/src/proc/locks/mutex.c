#include <proc/locks/mutex.h>
#include <io/uart.h>
extern void lock_mutex(void* mutex);
extern void unlock_mutex(void* mutex);


void take_lock(void* mutex){
    uart_putln(">>trying to take the lock");
    lock_mutex(mutex);
    uart_putln(">>lock taked");
}

void free_lock(void* mutex){
    uart_putln(">trying to free the lock");
    unlock_mutex(mutex);
    uart_putln(">lock freed");
}