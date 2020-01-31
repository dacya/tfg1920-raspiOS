#include "include/utils/mm.h"

.section ".text.boot"

// The general purpose registers are called by the name of
// Xn (for 64bits) or Wn (for 32 bits) ([0 <= n <= 30])

.globl _start
_start:
	mrs	x0, mpidr_el1
	//something called multiprocessor affinity register EL1
	//it has something to do with multi core identification
	//i think it is telling the hw to only use one core
	and	x0, x0,#0xFF		// Check processor id
	cbz	x0, master	  	// Hang for all non-primary CPU
	//cbz means compare and branch zero
	b	proc_hang

proc_hang:
	b 	proc_hang

master:
	adr	x0, bss_begin // it read the address of bss_begin
	//adr (address of page) it an instruction that read the PC
	//register value for bss_begin and stores it on x0
	//(it does more stuff before storing the value)
	adr	x1, bss_end   // and stores it in x0
	sub	x1, x1, x0
	bl 	memzero

	mov	sp, #LOW_MEMORY
	bl	kernel_main
	b 	proc_hang		// should never come here
