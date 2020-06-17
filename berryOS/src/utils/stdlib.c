/**
 * 
 * Raúl Sánchez Montaño
 * 
 * Dic 16, 2019
 */
#include <utils/stdlib.h>

int strlen(char* string) {
	int i = 0;
	while (string[i++] != '\0');
	return i - 1;
}

int streq(char* str1, char* str2) {
	while (*str1 == *str2) {
		if (*str1 == '\0')
			return 1;
		str1++;
		str2++;
	}
	return 0;
}


void memcpy(void * dest, void * src, int size){
	char * d  = dest;
	char * s = src;

	while(size--)
		*d++ = *s++;
}

void bzero2(void * dest, int size){
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
