#include "uart.h"
#include <avr/io.h>

void initUART(void) {
    UBRR0H = (BAUD_PRESCALE >> 8);
    UBRR0L = BAUD_PRESCALE;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void sendChar(char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void sendString(const char *str) {
    while (*str != '\0') {
        sendChar(*str);
        str++;
    }
}

char receiveSerialData(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void receiveUserInput(char *inputBuffer, int bufferSize) {
    int index = 0;
    char receivedChar;
    while (1) {
        while (!(UCSR0A & (1 << RXC0)));
        receivedChar = UDR0;
        if (receivedChar == '\b' && index > 0) {
            index--;
            inputBuffer[index] = '\0';
        } else if (receivedChar == '\r' || receivedChar == '\n') {
            inputBuffer[index] = '\0';
            break;
        } else if (index < bufferSize - 1) {
            inputBuffer[index++] = receivedChar;
            inputBuffer[index] = '\0';
        }
        sendChar(receivedChar);
    }
}