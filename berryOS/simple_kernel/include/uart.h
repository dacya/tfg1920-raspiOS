#ifndef	_PL011_UART_H
#define	_PL011_UART_H

#include "./peripherals/uart.h"

void uart_init ( void );
char uart_recv ( void );
void uart_send ( char c );
void uart_send_string(char* str);

#endif  /*_PL011_UART_H */