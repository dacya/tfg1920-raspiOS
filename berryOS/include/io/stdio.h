#ifndef _STDIO_H
#define _STDIO_H

#include <utils/color.h>

void print(char* text);
void printLn(char* text);
void enrichedPrint(char* text, color_24* txtColor, color_24* bgColor);
void enrichedPrintLn(char* text, color_24* txtColor, color_24* bgColor);

#endif /* _STDIO_H */