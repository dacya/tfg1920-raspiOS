#include <io/utils.h>
#include <io/peripherals/mini_uart.h>
#include <io/peripherals/gpio.h>

void uart_send ( char c )
{
	//we have to do a wait here or somethin with AUX_MU_LSR_REG

	put32(AUX_MU_IO_REG,c);
}

char uart_recv ( void )
{
	//we have to do a wait here or something with AUX_MU_LSR_REG
	return(get32(AUX_MU_IO_REG)&0xFF);
}

void uart_send_string(char* str)
{
	for (int i = 0; str[i] != '\0'; i ++) {
		uart_send((char)str[i]);
	}
}

void uart_init ( void )
{
	unsigned int selector = 0;

	selector = get32(GPFSEL1);
	//selector &= ~(7<<12);                   // clean gpio14
	selector |= (2<<12);                      // set alt5 for gpio14
	//selector &= ~(7<<15);                   // clean gpio15
	selector |= (2<<15);                      // set alt5 for gpio15
	put32(GPFSEL1,selector);

	put32(GPPUD,0);
	delay(150);
	put32(GPPUDCLK0,((1<<14)|(1<<15)));
	put32(GPPUDCLK1,0);
	delay(150);
	put32(GPPUD,0);
	put32(GPPUDCLK0,0);
	put32(GPPUDCLK1,0);

	/*maybe i should be using AUX_MU_IIR_REG register?
		on read just set bit 2:1 to 00
		on write, writing with bit 1 set, it clears the receive FIFO. with bit
		2 set, it clears the transmit FIFO
		the rest leave it as it is.
	*/
	put32(AUX_ENABLES,1);                   //Enable mini uart (this also enables access to it registers)
	put32(AUX_MU_CNTL_REG,0);               //Disable auto flow control and disable receiver and transmitter (for now)
	put32(AUX_MU_IER_REG,0);                //Disable receive and transmit interrupts
	put32(AUX_MU_LCR_REG,3);                //Enable 8 bit mode
	put32(AUX_MU_MCR_REG,0);                //Set RTS line to be always high
	put32(AUX_MU_BAUD_REG,270);             //Set baud rate to 115200

	put32(AUX_MU_CNTL_REG,3);               //Finally, enable transmitter and receiver
}
