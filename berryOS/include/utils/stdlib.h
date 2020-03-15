
/**
 * This library contains basic methods for register management
 * and string conversion.
 * 
 * Raúl Sánchez Montaño
 * 
 * Dic 16, 2019
 */



#ifndef STDLIB_H
#define STDLIB_H

#define MIN(x,y) ((x < y ? x : y))
#define MAX(x,y) ((x < y ? y : x))

void memcpy(void * dest, void * src, int size);

void bzero2(void * dest, int size);

char * itoa(int i);

int atoi(char * i);

int strlen(char* string);

#endif
