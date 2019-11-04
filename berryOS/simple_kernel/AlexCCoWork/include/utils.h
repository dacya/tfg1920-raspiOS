#ifndef	_BOOT_H
#define	_BOOT_H

extern void delay ( unsigned long); //parameters: (address)
extern void put32 ( unsigned long, unsigned int ); //parameters: (address, value)
extern unsigned int get32 ( unsigned long ); //parameters: (address)

#endif  /*_BOOT_H */
