#include "mini_uart.h"
#include "led.h"

void kernel_main(void)
{	
	char a;
	uart_init();
	led_init();
	led_set(LOW);

	uart_send_string("Hello, world!\r\n");

	while (1) {
		a = uart_recv();
		uart_send_string("Your character is: ");
		uart_send(a);

		if(a == 'o'){
			uart_send_string(", and it should turn on the led\n");
			led_set(HIGH);
		}
		else if (a == 'f'){
			uart_send_string(", and it should turn off the led\n");
			led_set(LOW);
		}
		else{
			uart_send_string("\n");
		}
		
	}
}
