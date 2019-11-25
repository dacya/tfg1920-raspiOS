#include <stdint.h> //for uint32_t
#include <io/peripherals/uart.h>
#include <io/gpio/gpio.h>
#include <utils/utils.h>

uint8_t uart_recv ( void ){
    while(*(UART0_FR)&(1<<4)); //wait while RX FIFO queue is not empty   
    return *(UART0_DR);
}
void uart_send (uint8_t c){
    while(*(UART0_FR)&(1<<5)); //wait while FIFO queue is full
    
    *(UART0_DR) &= (~0xFF);
    *(UART0_DR) |= c;
}

void uart_send_string(char* str){
    for(int i = 0; *(str) != '\0'; i++){
        uart_send(str[i]);
    }
}

void uart_init (unsigned char gpio_pin, enum pin_function function){

    if (gpio_pin == 14 || gpio_pin == 15 || gpio_pin == 16 || gpio_pin == 17 ||
        gpio_pin == 30 || gpio_pin == 31 || gpio_pin == 32 || gpio_pin == 33 ||
        gpio_pin == 36 || gpio_pin == 37 || gpio_pin == 38 || gpio_pin == 39){}
    else {
        return ;
    }
    /*
    Which one could be faster?
	switch (gpio_pin){
        case 14:;
        case 15:;
        case 16:;
        case 17:;
        case 30:;
        case 31:;
        case 32:;
        case 33:;
        case 36:;
        case 37:;
        case 38:;
        case 39: break;
        default: return;
    }*/
    
	volatile unsigned* reg_obj;

    switch (gpio_pin / 10){
		case 1:	reg_obj = GPFSEL1; break;
		default: reg_obj = GPFSEL3; break;
    }

	uint8_t shift = (gpio_pin % 10)*3;
    uint32_t selector = 0;

    *(UART0_CR) = selector; //we disable UART

    selector = *(reg_obj);  //we obtaine the GPFSELn register

	selector &= ~(0b111 << shift); // clean gpio_pin
	selector |= (function << shift);  // set gpio_pin method of work for gpio14
	
	*(reg_obj) = selector;

    /*
      this method of changing the pull-ups and timers is defined in page 101
      of documentation from BCM2837
    */

    // *(GPPUD) = 0; //we disable the Pull-up/pull down resistors
	// delay(150);
    // *(GPPUDCLK0) = (1<<14)|(1<<15); //we attach the timer 0 to GPIO pin 14 and 15
	// *(GPPUDCLK1) = 0; //it does nothing
	// delay(150);
    // *(GPPUD) = 0;
    // *(GPPUDCLK0) = 0;
    // *(GPPUDCLK1) = 0;

    pin_switch_pud(14, 0b00);
	pin_switch_pud(15, 0b00);

    *(UART0_ICR) = 0x7FF; //we clear every pending interrupt
    
    /*
      now whe should set the IBRD and FBRD registers.
      The integer baud rate register uses the result of INTEGER= UART_CLK / (16*baud)

      The fractional baud rate register uses the value of FR
      tmp = (((UART_CLK % (16 * baud)) * 8) / baud)
      FR = (tmp << 1) + (tmp & 1)
      OR
      FR = fractional part of (INTEGER *64) +0.5

      where UART_CLK = 3000000 (3MHZ) and baud is some of your choice.
    */
    //using baud as 115200, INTEGER = 3000000 / (16*115200) = 1.627 ~ 1
    *(UART0_IBRD) = 1;
    //calculating FR = (0.627 *64) +0.5 = 40.6 ~ 40
    *(UART0_FBRD) = 40;

    selector = 0;
    selector = (7<<4); //8 bits each word and FIFO enable
    *(UART0_LCRH) = selector;

    selector = 0;
    selector |= (1<<1); //CTS modem interrupt mask set
    selector |= (1<<4); //Receive interrupt mask set
    selector |= (1<<5); //TX interrupt mask set
    selector |= (1<<6); //Receive timeout interrupt mask set
    selector |= (1<<7); //Framing error interrupt mask set
    selector |= (1<<8); //Parity error interrupt mask set
    selector |= (1<<9); //Break error interrupt mask set
    selector |= (1<<10); //Overrun error interrupt mask set

    *(UART0_IMSC) = selector;

    selector = 0;
    //selector |= (1<<15); //Clear To Send Hardware flow control enable
    //selector |= (1<<14); //Request To Send Hardware flow control enable
    selector |= (1<<9); //receive enable
    selector |= (1<<8); //transmit enable
    selector |= 1; //uart enable

    *(UART0_CR) = selector;
}

/**
 * 
 * 
 * // Loop <delay> times in a way that the compiler won't optimize away
static inline void delay(int32_t count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
            : "=r"(count): [count]"0"(count) : "cc");
}

static inline void mmio_write(uint32_t reg, uint32_t data);

static inline uint32_t mmio_read(uint32_t reg);

*/