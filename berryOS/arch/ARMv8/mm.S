.globl memzero
memzero:
	str xzr, [x0], #8
	//xzr is called the read zero or discard result register
	//it stores zeores into the x0 + #8 address and sometimes
	//it is called the 31th register of general purpose
	subs x1, x1, #8   // (for 64 bits, 32 it's wzr)
	//next address
	b.gt memzero
	ret
