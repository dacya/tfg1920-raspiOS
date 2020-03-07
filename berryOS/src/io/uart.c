/**
 * 
 * Alejandro Cancelo Correia
 * 
 * Nov, 26, 2019
*/

#include <io/uart.h>
#include <io/gpio.h>
#include <stddef.h>

void uart_init() {
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
    pin_set_function(12, ALT5);
    pin_set_function(15, ALT5);

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

void uart_putc(unsigned char c) {
    // Wait for UART to become ready to transmit.
    while ( *(UART0_FR) & (1 << 5) );
    /* this doesn't work and i don't know why
    *(UART0_DR) &= (~0xFF);
    *(UART0_DR) |= (uint8_t)c;
    */

   *(UART0_DR) = c;
}

unsigned char uart_getc() {
    // Wait for UART to have received something.
    while ( *(UART0_FR) & (1 << 4) ) { }
    return *(UART0_DR);
}

void uart_puts(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++)
        uart_putc((unsigned char)str[i]);
}

void convert_to_str(unsigned int value, char *buff, int size) {
    char reminder;
    while(value != 0 && size > 0){
        reminder = value & 0xF;
        value = value >> 4;
        if(reminder < 10){
            buff[--size] = 48 + reminder;
        }
        else {
            buff[--size] = 55 + reminder;
        }
    }
}

void uart_hex_puts(uint32_t value) {
    char str_argument[9] = {'0','0','0','0','0','0','0','0', '\0'};
    convert_to_str(value, str_argument, 8);
    uart_puts("0x");
    uart_puts(str_argument);
    uart_puts("\r\n");
}

void clean_buf(char *buf, int size) {
    for(int i = 0; i < size; i++) {
        buf[i] = 0;
    }
}
