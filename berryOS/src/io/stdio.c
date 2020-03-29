#include <io/stdio.h>
#include <ui/views/console_view.h>
#include <io/uart.h>

void print(char* text) {
    //uart_puts(text);
    console_putStr(text, NULL, NULL);
}

void enrichedPrint(char* text, color_24* txtColor, color_24* bgColor) {
    //uart_puts(text);
    console_putStr(text, txtColor, bgColor);
}

void printLn(char* text) {
    //uart_putln(text);
    console_putStr(text, NULL, NULL);
    console_putLn("", NULL, NULL);
}

void enrichedPrintLn(char* text, color_24* txtColor, color_24* bgColor) {
    //uart_putln(text);
    console_putStr(text, txtColor, bgColor);
    console_putLn("", NULL, NULL);
}