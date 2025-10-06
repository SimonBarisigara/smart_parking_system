#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void lcd_init(void);      // Fridge LCD
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_data_print(char info[]);
void lcd_clear(void);

void lcd_init2(void);     // Gate LCD
void lcd_cmd2(unsigned char cmd);
void lcd_data2(unsigned char data);
void lcd_data_print2(char info[]);
void lcd_clear2(void);

#endif