/*
	This file is intended to be used by the Raspberry Pi 2
 */

.globl put32
put32:
    stmed sp!,{lr}
	str r1,[r0]
    ldmfa sp!,{lr}
	mov pc, lr

.globl get32
get32:
    stmed sp!,{lr}
	ldr r0,[r0]
    ldmfa sp!,{lr}
	mov pc, lr

.globl delay
delay:
    stmed sp!,{lr}
	subs r0, r0, #1
	bne delay
    ldmfa sp!,{lr}
	mov pc, lr