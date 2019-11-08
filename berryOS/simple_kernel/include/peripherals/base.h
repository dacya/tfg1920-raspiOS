#ifndef	_P_BASE_H
#define	_P_BASE_H

#include <stdint.h>
/*                            IMPORTANT

 We will be using this memory base value because we'll be working directly
 to the ARM peripherals (see section 1.2.3 from BCM2837 or BCM2836 or BCM2835
 peripherals documentation for more info)

*/
#define PBASE 0x3F000000

#endif  /*_P_BASE_H */
