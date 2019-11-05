#ifndef	_BOOT_H
#define	_BOOT_H

/*
    parameters: (num cycles)
    This function takes a numer of cycles to be stopped
*/
extern void delay ( unsigned long); 
/*
    parameters: (address, value)
    This function puts 32 bits into the specified address
*/
extern void put32 ( unsigned long, unsigned int ); 
/*
    parameters: (address)
    This function obtains the 32 bits from an address
*/
extern unsigned int get32 ( unsigned long ); 

#endif  /*_BOOT_H */
