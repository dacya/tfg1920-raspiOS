/*
	This file is intended to be used by the Raspberry Pi 2
 */

.globl put32
put32:
    stmea sp!,{lr}
	str r1,[r0]
    ldmfd sp!,{lr}
	mov pc, lr

.globl get32
get32:
    stmea sp!,{lr}
	ldr r0,[r0]
    pop{lr}
    ldmfd sp!,{lr}
	mov pc, lr

.globl delay
delay:
    stmea sp!,{lr}
	subs r0, r0, #1
	bne delay
    ldmfd sp!,{lr}
	mov pc, lr