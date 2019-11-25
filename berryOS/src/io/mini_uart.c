#include <stdint.h>
#include <io/peripherals/mini_uart.h>
#include <io/peripherals/gpio.h>
#include <utils/utils.h>

void mini_uart_send ( uint8_t c )
{	
	while(1){
		if(*(AUX_MU_LSR)&0x40){
			break;
		}
	}
	*(AUX_MU_IO) &= (~0xFF);
	*(AUX_MU_IO) |= c;	
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

void mini_uart_init (unsigned char gpio_pin)
{

	if(gpio_pin != 14 && gpio_pin != 15 && gpio_pin != 32 && gpio_pin != 33 &&
	   gpio_pin != 40 && gpio_pin != 41)
		return;
	
	volatile unsigned* reg_obj;
	
	switch (gpio_pin / 10){
		case 1:	reg_obj = GPFSEL1; break;
		case 3: reg_obj = GPFSEL3; break;
		default: reg_obj = GPFSEL4; break;
    }

	uint32_t selector = 0;
	uint8_t shift = (gpio_pin % 10)*3;

	selector = *(reg_obj);
	selector &= ~(7<<shift);                   // clean gpio14
	selector |= (2<<shift);                    // set alt5 for gpio14
	selector &= ~(7<<shift);                   // clean gpio15
	selector |= (2<<shift);                    // set alt5 for gpio15
	
	*(reg_obj) = selector;

	*(GPPUD) = 0;
	delay(150);
	*(GPPUDCLK0) = (1<<14)|(1<<15);
	*(GPPUDCLK1) = 0;
	delay(150);
	*(GPPUD) = 0;
	*(GPPUDCLK0) = 0;
	*(GPPUDCLK1) = 0;

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
