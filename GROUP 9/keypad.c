#include "keypad.h"
#include <avr/io.h>

int keypad(void) {
    PORTL = 0b11111011;
    if ((PINL & 0b00010000) == 0) return 1;
    if ((PINL & 0b00100000) == 0) return 4;
    if ((PINL & 0b01000000) == 0) return 7;
    if ((PINL & 0b10000000) == 0) return 100;

    PORTL = 0b11111101;
    if ((PINL & 0b00010000) == 0) return 2;
    if ((PINL & 0b00100000) == 0) return 5;
    if ((PINL & 0b01000000) == 0) return 8;
    if ((PINL & 0b10000000) == 0) return 0;

    PORTL = 0b11111110;
    if ((PINL & 0b00010000) == 0) return 3;
    if ((PINL & 0b00100000) == 0) return 6;
    if ((PINL & 0b01000000) == 0) return 9;
    if ((PINL & 0b10000000) == 0) return 1000;

    return 200;
}