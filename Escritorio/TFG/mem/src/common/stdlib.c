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

	if(i == 0){
		buf[0] = '0';
		buf[1] = '\0';
		return buf;
	}

	while(i != 0){
		buf[j++] = 48 + i % 10;
		i = i / 10;
	}

    char aux;
    int size = j;
    j--;
    while(i < j){
        aux = buf[j];
        buf[j--] = buf[i];
        buf[i++] = aux;
    }

	buf[size] = '\0';	
	
	return buf;

}

int positiveAtoi(char * asc){
	int res = 0;

	for(int i = 0; asc[i] != '\0'; i++){
		res += res * 10 + asc[i] - '0';
	}

	return res;

}