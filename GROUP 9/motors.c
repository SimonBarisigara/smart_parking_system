#include "motors.h"

void buzzer(void) {
    DDRD |= (1 << 1);
    PORTD |= (1 << 1);
    _delay_ms(500);
    PORTD &= ~(1 << 1);
    _delay_ms(1800);
    stopBuzer();
}

void stopBuzer(void) {
    DDRD &= ~(1 << 1);
}

void gateopen(void) {
    PORTC |= (1 << 0) | (1 << 2);
    _delay_ms(3000);
    PORTC &= ~(1 << 0);
}

void gateclose(void) {
    PORTC &= ~(1 << 0);
    PORTC |= (1 << 0) | (1 << 2);
    _delay_ms(2000);
    PORTC &= ~(1 << 0);
}

void servo(int rotations) {
    DDRJ |= (1 << 0);
    PORTJ = 0x00;
    for (int i = 0; i < rotations; ++i) {
        PORTJ |= (1 << 0); _delay_us(1000); PORTJ = 0x00; _delay_ms(1000);
        PORTJ |= (1 << 0); _delay_us(2000); PORTJ = 0x00; _delay_ms(1000);
        PORTJ |= (1 << 0); _delay_us(3000); PORTJ = 0x00; _delay_ms(1000);
    }
}