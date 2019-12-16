/**
 *                             IMPORTANT
 * We will be using this memory base value because we'll be working directly
 * to the ARM peripherals (see section 1.2.3 from BCM2837 or BCM2836 or BCM2835
 * peripherals documentation for more info)
 */

#ifndef	_P_BASE_H
#define	_P_BASE_H

#include <stdint.h>

#define PHYSICAL_PBASE ((uint32_t)0x3F000000)
#define VIRTUAL_PBASE  ((uint32_t)0x7E000000)

#define MAILBOX_OFFSET 0xB880


#endif  /*_P_BASE_H */
