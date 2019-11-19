#include <stdint.h> //for uint32_t
#include <io/peripherals/uart.h>
#include <io/peripherals/gpio.h>
#include <utils/utils.h>

char uart_recv ( void ){
    while(*(UART0_FR)&(1<<4)); //wait while RX FIFO queue is not empty
    return *(UART0_DR);
}
void uart_send ( char c ){
    while(*(UART0_FR)&(1<<5)); //wait while FIFO queue is full

    *(UART0_DR) = c;
}

void uart_send_string(char* str){
    for(int i = 0; *(str) != '\0'; i++){
        uart_send(str[i]);
    }
}

void uart_init ( void ){
    uint32_t selector;

    *(UART0_CR) = 0; //we disable UART
    /*
    selector = *(GPFSEL1);  //we obtaine the GPFSEL1 register

	selector &= ~(7<<12); // clean gpio14
	selector |= (4<<12);  // set alt0 for gpio14
	selector &= ~(7<<15); // clean gpio15
	selector |= (4<<15);  // set alt0 for gpio15
	
	*(GPFSEL1) = selector;
    */
    /*
      this method of changing the pull-ups and timers is defined in page 101
      of documentation from BCM2837
    */
    *(GPPUD) = 0; //we disable the Pull-up/pull down resistors
	delay(150);
    *(GPPUDCLK0) = (1<<14)|(1<<15); //we attach the timer 0 to GPIO pin 14 and 15
	*(GPPUDCLK1) = 0; //it does nothing
	delay(150);
    *(GPPUD) = 0;
    *(GPPUDCLK0) = 0;
    *(GPPUDCLK1) = 0;

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