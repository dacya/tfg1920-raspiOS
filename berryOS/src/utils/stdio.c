
#include <io/uart.h>
#include <utils/stdio.h>

char getc(void){
	return uart_getc();
}

void putc(char c){
	uart_putc(c);
}

void puts(const char * str){
	for(int i = 0; str[i] != '\0'; i++)
		putc(str[i]);
}

void gets(char * buf, int buflen){
	int i;
	char c;	
	for (i = 0; (c = getc()) != '\r' && buflen > 1; i++, buflen--){
   
        buf[i] = c;
    	}

    	putc('\n');
    	if (c == '\n')
   		buf[i] = '\0';
    	else
        	buf[buflen-1] = '\0';	
}
