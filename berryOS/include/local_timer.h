/*
* This is a simple library to ease the use of arm local timers
* 
* Alejandro Cancelo Correia
*
* April, 1, 2020
*/
#ifndef _LOCAL_TIMER_H
#define _LOCAL_TIMER_H

#include <interrupts.h>

#define QUANTUM 50

/**
 * We can select the counter to trigger interrupts.
 * Keep in mind the should be an offset with every virtual counter
*/
typedef enum{
    PHYSICAL_SYS,
    VIRTUAL_SYS,
    PHYSICAL_HIPER
} timer_selection_t;

/**
 * It initialize the local timer for core 0 to a specified value
 * In order to make an appropiate use of this function you must to use calculate_time()
 * function
 * @param local_timer A selection of the timer to use. It can be
 *  PHYSICAL_SYS, VIRTUAL_SYS
 * @param time an unsigned 32 bit value indicating the period time in milisecons of each interrupt
 * @return it returns -1 on error and 0 if everything went well.
 * @see calculate_time()
*/
uint32_t local_timer_init(timer_selection_t local_timer_select, unsigned int time);

/**
 * Read the physical count. This values is updated by the system counter
*/
uint64_t read_CNTPCT(void);

uint32_t read_CNTFRQ(void);

/**
 * Read the Compare Value of one of each counters (physical or virtual)
 * @return The compare value or -1 if an error occurs
*/
uint64_t read_CNTX_CVAL(timer_selection_t local_timer_select);

/**
 * Writes to the Compare Value of one of each counter (physical or virtual)
 * @param local_timer_selec the timer to read the control of
 * @param compare the value to compare for trigger an event
*/
void write_CNTX_CVAL(timer_selection_t local_timer_select, uint64_t compare);

/**
 * Reads the Timer Value register of one of each counters(physical or virtual)
 * @return The timer value or -1 if an error occurs
*/
uint32_t read_CNTX_TVAL(timer_selection_t local_timer_select);

/**
 * Write to the Timer Value register of one of each counters (physical or virtual)
 * @param local_timer_select the timer to write the value in
 * @param value the value to writes
*/
void write_CNTX_TVAL(timer_selection_t local_timer_select, uint32_t value);

/**
 * Read the Control register of one of each counters (physical or virtual)
 * @return The control status or -1 if an error occurs
*/
uint32_t read_CNTX_CTL(timer_selection_t local_timer_selec);

/**
 * Writes to the Control of one of each counter (physical or virtual)
 * @param local_timer_selec the timer to read the control of
 * @param flags flags to activate
*/
void write_CNTX_CTL(timer_selection_t local_timer_selec, uint32_t flags);

#endif