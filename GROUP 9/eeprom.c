#include "eeprom.h"
#include <avr/io.h>

char EEPROM_read(uint16_t uiAddress) {
    while (EECR & (1 << EEPE));
    EEARH = (uiAddress & 0xFF00) >> 8;
    EEARL = (uiAddress & 0x00FF);
    EECR |= (1 << EERE);
    return EEDR;
}

void EEPROM_write(uint16_t uiAddress, char ucData) {
    while (EECR & (1 << EEPE));
    EEARH = (uiAddress & 0xFF00) >> 8;
    EEARL = (uiAddress & 0x00FF);
    EEDR = ucData;
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
}