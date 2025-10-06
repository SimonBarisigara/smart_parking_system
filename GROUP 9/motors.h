#ifndef MOTORS_H
#define MOTORS_H

#include <avr/io.h>
#include <util/delay.h>

void buzzer(void);
void stopBuzer(void);
void gateopen(void);
void gateclose(void);
void servo(int rotations);

#endif