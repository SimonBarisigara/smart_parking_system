#ifndef EEPROM_H
#define EEPROM_H

char EEPROM_read(uint16_t uiAddress);
void EEPROM_write(uint16_t uiAddress, char ucData);

#endif