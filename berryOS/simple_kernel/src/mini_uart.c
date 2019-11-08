#include <stdint.h>
#include "../include/mini_uart.h"
#include "../include/peripherals/peripherals_variables.h"
#include "../include/utils.h"

void mini_uart_send ( char c )
{
	while(1){
		if(*(AUX_MU_LSR_REG)&0x40){
			break;
		}
	}

	*(AUX_MU_IO_REG) = c;
}

//void _uart_send_register(  );
char mini_uart_recv ( void )
{
	//we have to do a wait here or something with AUX_MU_LSR_REG
	char error = '0';
	unsigned int registerValue;
	while(1){
		registerValue = *(AUX_MU_LSR_REG);
		if(registerValue&0x01){
			if(registerValue&0x02){
				error = '1';
			}
			break;
		}
	}

	mini_uart_send(error);
	return(*(AUX_MU_IO_REG)&0xFF);
}

void mini_uart_send_string(char* str)
{
	for (int i = 0; str[i] != '\0'; i ++) {
		mini_uart_send((char)str[i]);
	}
}

void mini_uart_init ( void )
{
	unsigned int selector = 0;

	selector = *(GPFSEL1_REG);
	selector &= ~(7<<12);                   // clean gpio14
	selector |= (2<<12);                    // set alt5 for gpio14
	selector &= ~(7<<15);                   // clean gpio15
	selector |= (2<<15);                    // set alt5 for gpio15
	
	*(GPFSEL1_REG) = selector;

	*(GPPUD_REG) = 0;
	delay(150);
	*(GPPUDCLK0_REG) = (1<<14)|(1<<15);
	*(GPPUDCLK1_REG) = 0;
	delay(150);
	*(GPPUD_REG) = 0;
	*(GPPUDCLK0_REG) = 0;
	*(GPPUDCLK1_REG) = 0;

	/*maybe i should be using AUX_MU_IIR_REG register?
		on read just set bit 2:1 to 00
		on write, writing with bit 1 set, it clears the receive FIFO. with bit
		2 set, it clears the transmit FIFO
		the rest leave it as it is.
	*/
	*(AUX_ENABLES_REG) = 1;                   //Enable mini uart (this also enables access to it registers)
	*(AUX_MU_CNTL_REG) = 0;               //Disable auto flow control and disable receiver and transmitter (for now)
	*(AUX_MU_IER_REG) = 0;                //Disable receive and transmit interrupts
	*(AUX_MU_LCR_REG) = 3;                //Enable 8 bit mode
	*(AUX_MU_MCR_REG) = 0;                //Set RTS line to be always high
	*(AUX_MU_BAUD_REG) = 270;             //Set baud rate to 115200

	*(AUX_MU_CNTL_REG) = 3;               //Finally, enable transmitter and receiver
}
