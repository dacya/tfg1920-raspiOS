#include <stddef.h>
#include <stdint.h>
#include <kernel/uart.h>
#include <common/stdio.h>
#include <common/stdlib.h>

volatile uint32_t* gpio_fs1 = (uint32_t*)(GPIO_BASE+0x04);

volatile uint32_t* gpio_set0 = (uint32_t*)(GPIO_BASE+0x1C);

volatile uint32_t* gpio_clr0 = (uint32_t*)(GPIO_BASE+0x28);

static inline void delay(int32_t count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
            : "=r"(count): [count]"0"(count) : "cc");
}



void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags){
    
	*gpio_fs1 |= (uint32_t)(1<<21);
	
	int32_t tim = 1000000;

	while (1) {
		
		*gpio_set0 |= (uint32_t)(1<<17);	
		
		delay(tim);

		*gpio_clr0 |= (uint32_t)(1<<17);		
		
		delay(tim);			
	}

}






/*
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags){
    
	*gpio_fs1 = *gpio_fs1 | (uint32_t)0b00000000001000000000000000000000;
	
	int32_t tim = 1000000;

	while (1) {
		
		*gpio_set0 = *gpio_set0 | (uint32_t)0b00000000000000100000000000000000;
		*gpio_set0 = *gpio_set0 & (uint32_t)0b11111111111111011111111111111111;
		
		delay(tim);

		*gpio_clr0 = *gpio_clr0 | (uint32_t)0b00000000000000100000000000000000;
		*gpio_clr0 = *gpio_clr0 & (uint32_t)0b11111111111111011111111111111111;
		
		delay(tim);			
	}

}
*/

