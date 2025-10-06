#ifndef UART_H
#define UART_H

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 8UL))) - 1)

void initUART(void);
void sendChar(char data);
void sendString(const char *str);
char receiveSerialData(void);
void receiveUserInput(char *inputBuffer, int bufferSize);

#endif