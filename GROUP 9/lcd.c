#include "lcd.h"

static void latch(void) {
    PORTG |= (1 << PG2);
    _delay_ms(50);
    PORTG &= ~(1 << PG2);
    _delay_ms(50);
}

void lcd_cmd(unsigned char cmd) {
    PORTH = cmd;
    PORTG &= ~(1 << PG0);
    latch();
}

void lcd_data(unsigned char data) {
    PORTH = data;
    PORTG |= (1 << PG0);
    PORTG &= ~(1 << PG1);
    latch();
}

void lcd_init(void) {
    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}

void lcd_data_print(char info[]) {
    int len = strlen(info);
    for (int a = 0; a < len; a++) {
        lcd_data(info[a]);
    }
}

void lcd_clear(void) {
    lcd_cmd(0x01);
}

static void latch2(void) {
    PORTB |= (1 << PB2);
    _delay_ms(50);
    PORTB &= ~(1 << PB2);
    _delay_ms(50);
}

void lcd_cmd2(unsigned char cmd) {
    PORTA = cmd;
    PORTB &= ~(1 << PB0);
    PORTB &= ~(1 << PB1);
    latch2();
}

void lcd_data2(unsigned char data) {
    PORTA = data;
    PORTB |= (1 << PB0);
    PORTB &= ~(1 << PB1);
    latch2();
}

void lcd_init2(void) {
    lcd_cmd2(0x38);
    lcd_cmd2(0x0C);
    lcd_cmd2(0x06);
    lcd_cmd2(0x01);
}

void lcd_data_print2(char info[]) {
    int len = strlen(info);
    for (int a = 0; a < len; a++) {
        lcd_data2(info[a]);
    }
}

void lcd_clear2(void) {
    lcd_cmd2(0x01);
}