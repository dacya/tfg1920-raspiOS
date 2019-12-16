
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

void memcpy(void * dest, void * src, int size);

void bzero(void * dest, int size);

char * itoa(int i);

int atoi(char * i);
#endif
