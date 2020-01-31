#include <stdint.h>
#include <peripherals/mini_uart.h>
#include <peripherals/gpio.h>

void mini_uart_transmit_reg( void ){
	uint32_t aux_mu_stat_register = *(AUX_MU_STAT);

	uint8_t transmitAndReceiveFIFOfill = (aux_mu_stat_register>>16)&0xF; //receive status
	transmitAndReceiveFIFOfill |= (((aux_mu_stat_register>>24)&0xF)<<4); //tx status
	//char firstByte = aux_mu_stat_register&0xFF;
	//char secondByte = (aux_mu_stat_register&0x0300)>>8;

	//*(AUX_MU_IO) = transmitAndReceiveFIFOfill;
	//*(AUX_MU_IO) = firstByte;
	//*(AUX_MU_IO) = secondByte;
	mini_uart_send(transmitAndReceiveFIFOfill);
}

void mini_uart_send ( uint8_t c )
{	
	while(1){
		if(*(AUX_MU_LSR)&0x40){
			break;
		}
	}

	*(AUX_MU_IO) = c;	
}

//void _uart_send_register(  );
uint8_t mini_uart_recv ( void )
{
	//we have to do a wait here or something with AUX_MU_LSR
	uint8_t error = '0';
	uint32_t registerValue;
	while(1){
		registerValue = *(AUX_MU_LSR);
		if(registerValue&0x01){
			if(registerValue&0x02){
				error = '1';
			}
			break;
		}
	}

	mini_uart_send(error);
	return(*(AUX_MU_IO)&0xFF);
}

void mini_uart_send_string(char* str)
{
	for (int i = 0; str[i] != '\0'; i ++) {
		mini_uart_send((uint8_t)str[i]);
	}
}

void mini_uart_init ( void )
{
	// uint32_t selector = 0;

	// selector = *(GPFSEL1);
	// selector &= ~(7<<12);                   // clean gpio14
	// selector |= (2<<12);                    // set alt5 for gpio14
	// selector &= ~(7<<15);                   // clean gpio15
	// selector |= (2<<15);                    // set alt5 for gpio15

	pin_set_function(12, ALT5);
	pin_set_function(15, ALT5);
	
	// *(GPFSEL1) = selector;

	// *(GPPUD) = 0;
	// delay(150);
	// *(GPPUDCLK0) = (1<<14)|(1<<15);
	// *(GPPUDCLK1) = 0;
	// delay(150);
	// *(GPPUD) = 0;
	// *(GPPUDCLK0) = 0;
	// *(GPPUDCLK1) = 0;

	pin_switch_pud(14, 0b00);
	pin_switch_pud(15, 0b00);

	/*maybe i should be using AUX_MU_IIR register?
		on read just set bit 2:1 to 00
		on write, writing with bit 1 set, it clears the receive FIFO. with bit
		2 set, it clears the transmit FIFO
		the rest leave it as it is.
	*/
	*(AUX_ENABLES) = 1;                   //Enable mini uart (this also enables access to it registers)
	*(AUX_MU_CNTL) = 0;               //Disable auto flow control and disable receiver and transmitter (for now)
	*(AUX_MU_IER) = 0;                //Disable receive and transmit interrupts
	*(AUX_MU_LCR) = 3;                //Enable 8 bit mode
	*(AUX_MU_MCR) = 0;                //Set RTS line to be always high
	*(AUX_MU_BAUD) = 270;             //Set baud rate to 115200

	*(AUX_MU_CNTL) = 3;               //Finally, enable transmitter and receiver
}
