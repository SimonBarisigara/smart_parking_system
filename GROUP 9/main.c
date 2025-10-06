#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include "lcd.h"
#include "keypad.h"
#include "eeprom.h"
#include "uart.h"
#include "motors.h"
#include "menu.h"

ISR(INT0_vect) {
    displayWelcome();
    _delay_ms(10);
    buzzer();
}

void car_approaching(void) {
    lcd_init2();
    lcd_cmd2(0x80);
    lcd_data_print2("CAR APPROACHING");
    _delay_ms(2000);
    lcd_clear2();
    registration();
    gate();
}

void registration(void) {
    lcd_init2();
    lcd_cmd2(0x80);
    lcd_data_print2("REGISTRATION IN PROGRESS.");
    _delay_ms(10000);
    lcd_clear2();
}

void gate(void) {
    lcd_init2();
    lcd_cmd2(0x80);
    lcd_data_print2("GATE OPENING.");
    _delay_ms(3000);
    gateopen();
    lcd_clear2();
    lcd_init2();
    lcd_cmd2(0x80);
    lcd_data_print2("CAR PASSING.");
    _delay_ms(3000);
    lcd_init2();
    lcd_cmd2(0x80);
    lcd_data_print2("GATE CLOSING.");
    _delay_ms(2000);
    lcd_clear2();
    gateclose();
}

void captureDrinks(void) {
    lcd_init();
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_data_print("FRIDGE.");
    lcd_cmd(0xC0);
    lcd_data_print("Number of drinks: ");
    int bottle = keypad();
    while (bottle < 0 || bottle > 9) {
        bottle = keypad();
    }
    char bottles[2];
    sprintf(bottles, "%d", bottle);
    lcd_cmd(0xD3);
    lcd_data_print(bottles);
    lcd_cmd(0x94);
    lcd_data_print("Amount: ");
    int amount = bottle * 1000;
    char amountString[5];
    sprintf(amountString, "%d", amount);
    lcd_cmd(0xA2);
    lcd_data_print(amountString);
    lcd_cmd(0xA6);
    lcd_data_print("/");
    lcd_cmd(0xA7);
    lcd_data_print("=");
    int command = keypad();
    while (command == 200) {
        command = keypad();
    }
    if (command == 1000) {
        servo(bottle);
    }
    if (command == 100) {
        _delay_ms(1000);
        lcd_cmd(0xD4);
        lcd_data_print("Canceled...");
        _delay_ms(1000);
    }
    _delay_ms(1000);
    lcd_cmd(0xD4);
    lcd_data_print("Order Again.");
    _delay_ms(1000);
}

void displayWelcome(void) {
    lcd_init2();
    lcd_cmd2(0x80);
    lcd_data_print2("WELl");
    _delay_ms(1000);
    lcd_clear2();
}

int main(void) {
    initUART();

    EEPROM_write(40, '1');
    _delay_ms(1000);
    EEPROM_write(41, '2');
    _delay_ms(1000);
    EEPROM_write(42, '3');
    _delay_ms(1000);

    virtualTerminal();

    while (1) {
        DDRL = 0b00000111;
        DDRG = 0xFF;
        DDRH = 0xFF;
        DDRA = 0xFF;
        DDRB = 0b00000111;
        DDRC = 0xFF;
        DDRK = 0x00;
        DDRD &= ~(1 << 0);
        EICRA |= (1 << INT0);
        EIMSK |= (1 << INT0);
        sei();

        performLogin();
        captureDrinks();
    }
    return 0;
}