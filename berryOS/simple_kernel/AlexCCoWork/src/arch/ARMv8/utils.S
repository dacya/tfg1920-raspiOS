/*
	This file is intended to be used by the Raspberry Pi 3
 */

.globl put32
put32:
	str w1,[x0]
	ret //default r30(it's the same as x30 or w30) if it's omitted
	    //and it is the link register (lr)

.globl get32
get32:
	ldr w0,[x0]
	ret

.globl delay
delay:
	subs x0, x0, #1
	bne delay
	ret
