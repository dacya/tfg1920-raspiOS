#ifndef	_MM_H
#define	_MM_H

#define PAGE_SHIFT	 	12
#define TABLE_SHIFT 	 9
#define SECTION_SHIFT	(PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE   	(1 << PAGE_SHIFT)	//0x00 00 00 00 00 00 01 00
#define SECTION_SIZE	(1 << SECTION_SHIFT)//0x00 00 00 00 00 20 00 00

#define LOW_MEMORY      (2 * SECTION_SIZE)  //0x00 00 00 00 00 40 00 00

#ifndef __ASSEMBLER__

void memzero(unsigned long src, unsigned long n);

#endif

#endif  /*_MM_H */
