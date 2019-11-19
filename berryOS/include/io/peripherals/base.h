#ifndef	_P_BASE_H
#define	_P_BASE_H
/*                            IMPORTANT

 We will be using this memory base value because we'll be working directly
 to the ARM peripherals (see section 1.2.3 from BCM2837 or BCM2836 or BCM2835
 peripherals documentation for more info)

*/
#define PHYSICAL_PBASE (0x3F000000)
#define VIRTUAL_PBASE  (0x7E000000)

#endif  /*_P_BASE_H */
