/* From https://github.com/dhepper/font8x8/blob/master/font8x8_block.h */

#ifndef CHAR_BMPS_H
#define CHAR_BMPS_H

#include <stdint.h>

extern int CHAR_MIN_SIZE;

const uint8_t* font(int c);

#endif