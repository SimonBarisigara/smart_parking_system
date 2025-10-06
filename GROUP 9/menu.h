#ifndef MENU_H
#define MENU_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "uart.h"
#include "eeprom.h"

void virtualTerminal(void);
void displayMenuLogin(void);
void displayMenu(void);
void choiceChecker(void);
void performRegistration(void);
void performLogin(void);
void logout(void);

#endif