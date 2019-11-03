#include <common/stdlib.h>

void memcpy(void * dest, void * src, int size){
	char * d  = dest;
	char * s = src;

	while(size--)
		*d++ = *s++;
}

void bzero(void * dest, int size){
	char * d  = dest;
	while(size--)
		*d++ = 0;
	
}

char * itoa(int i){
	static char buf[12];
	int j = 0;
	int neg = 0;

	if(i == 0){
		buf[0] = '0';
		buf[1] = '\0';
		return buf;
	}
	if(i < 0){
		neg = 1;
		i = -i;
	}

	while(i != 0){
		buf[j++] = 48 + i % 10;
		i = i / 10;
	}
	
	if(neg)
		buf[j] = '-';

	buf[j + 1] = '\0';	

	while(i < j){
		neg = buf[i];
		buf[i++] = buf[j];
		buf[j--] = neg; 
	}

	return buf;

}
