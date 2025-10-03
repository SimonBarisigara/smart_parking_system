/*
 * Project.c
 *
 * Created: 26/10/2023 11:16:42
 * Author : Simon
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include <string.h>
#ifndef F_CPU
#define F_CPU 8000000UL

#endif
#define MAX_STRING_LENGTH 5


ISR(INT0_vect)
{
	displayWelcome();
	_delay_ms(10);
	
	buzzer();
	
	
}

char EEPROM_read(uint16_t uiAddress)
{
	
	// if(uiAddress > EEPROM_SIZE){
	// 	return EEPROM_INVALID_ADDR;
	// }
	
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));
	
	EEARH = (uiAddress & 0xFF00) >> 8;
	EEARL = (uiAddress & 0x00FF);
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	
	return EEDR;
}


void EEPROM_write(uint16_t uiAddress, char ucData)
{
	
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));
	/* Set up address and Data Registers */
	EEARH = (uiAddress & 0xFF00) >> 8;
	EEARL = (uiAddress & 0x00FF);
	EEDR = ucData;
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);

}

// function for keypad at the Fridge
int keypad()
{
	PORTL = 0b11111011;
	if ((PINL & 0b00010000) == 0)
	{
		return 1;
	}
	if ((PINL & 0b00100000) == 0)
	{
		return 4;
	}
	if ((PINL & 0b01000000) == 0)
	{
		return 7;
	}
	if ((PINL & 0b10000000) == 0)
	{
		return 100;
	}

	PORTL = 0b11111101;
	if ((PINL & 0b00010000) == 0)
	{
		return 2;
	}
	if ((PINL & 0b00100000) == 0)
	{
		return 5;
	}
	if ((PINL & 0b01000000) == 0)
	{
		return 8;
	}
	if ((PINL & 0b10000000) == 0)
	{
		return 0;
	}

	PORTL = 0b11111110;
	if ((PINL & 0b00010000) == 0)
	{
		return 3;
	}
	if ((PINL & 0b00100000) == 0)
	{
		return 6;
	}
	if ((PINL & 0b01000000) == 0)
	{
		return 9;
	}
	if ((PINL & 0b10000000) == 0)
	{
		return 1000;
	}

	return 200;
}
// funtion to  latch data at the gate
void latch2()
{
	PORTB |= (1 << PB2);
	_delay_ms(50);
	PORTB &= ~(1 << PB2);
	_delay_ms(50);
}

// function to send comnmands to the LCD for the gate
void lcd_cmd2(unsigned char cmd2)
{
	PORTA = cmd2;
	PORTB &= ~(1 << PB0); 
	PORTB &= ~(1 << PB1); 
	latch2();
}

// function to send data to the LCD for the gate
void lcd_data2(unsigned char data2)
{
	PORTA = data2;
	PORTB |= (1 << PB0);  
	PORTB &= ~(1 << PB1); 
	latch2();
}

// function to initialize the LCD for the gate
void lcd_init2()
{
	lcd_cmd2(0x38); 
	lcd_cmd2(0x0c); 
	lcd_cmd2(0x06); 
	lcd_cmd2(0x01); 
}

// function to print data to the LCD for the gate
void lcd_data_print2(char info2[])
{
	int len2 = strlen(info2);
	for (int a = 0; a < len2; a++)
	{
		lcd_data2(info2[a]);
	}
}

// function to latch the data for FRIGE
void latch()
{
	PORTG |= (1 << PG2);
	_delay_ms(50);
	PORTG &= ~(1 << PG2);
	_delay_ms(50);
}

// function to send comnmands to the LCD for FRIDGE
void lcd_cmd(unsigned char cmd)
{
	PORTH = cmd;
	PORTG &= ~(1 << PG0); 
	latch();
}

// function to send data to the LCD for FRIDGE
void lcd_data(unsigned char data)
{
	PORTH = data;
	PORTG |= (1 << PG0);  
	PORTG &= ~(1 << PG1); 
	latch();
}

// function to initialize the LCD for FRIDGE
void lcd_init()
{
	lcd_cmd(0x38);
	lcd_cmd(0x0c); 
	lcd_cmd(0x06); 
	lcd_cmd(0x01); 
}

// function to print data to the LCD for FRIDGE
void lcd_data_print(char info[])
{
	int len = strlen(info);
	for (int a = 0; a < len; a++)
	{
		lcd_data(info[a]);
	}
}
// function to capture the drinks
void captureDrinks()
{
	lcd_init();
	lcd_cmd(0x01); 

	lcd_cmd(0x80); 
	lcd_data_print("FRIDGE.");
	latch();

	lcd_cmd(0xC0); 
	lcd_data_print("Number of drinks: ");
	latch();
	int bottle = keypad();
	while (bottle < 0 || bottle > 9)
	{
		bottle = keypad();
	}
	char bottles[2]; 
	sprintf(bottles, "%d", bottle);
	lcd_cmd(0xD3);
	lcd_data_print(bottles);
	latch();

	// Display the Amount and prompt confirmation // NOTE that every bottle is 1000 shillings
	lcd_cmd(0x94); // row 3
	lcd_data_print("Amount: ");
	// Assuming each bottle costs 1000 shillings
	int amount = bottle * 1000;
	char amountString[5]; // Size 5 to hold up to 9999
	sprintf(amountString, "%d", amount);
	lcd_cmd(0xA2);
	lcd_data_print(amountString);
	lcd_cmd(0xA6);
	lcd_data_print("/");
	latch();
	lcd_cmd(0xA7);
	lcd_data_print("=");
	latch();

	int command = keypad();
	while (command == 200)
	{
		command = keypad();
	}

	// evaluate the command
	if (command == 1000)
	{
		// Call the servo function to dispense the drinks for the specified number of rotations
		servo(bottle);
	}
	if (command == 100)
	{
		// Display the thank you message after confirmation
		_delay_ms(1000);
		lcd_cmd(0xD4); // row
		lcd_data_print("Canceled...");
		_delay_ms(1000);
		latch();
	}

	// Display the thank you message after confirmation
	_delay_ms(1000);
	lcd_cmd(0xD4); // row
	lcd_data_print("Order Again.");
	_delay_ms(1000);
	latch();
}

//*********************************************************************************************************************************************************************
// function to display the welcome message


void displayWelcome()
{

	lcd_init2();
	lcd_cmd2(0x80); // row 1
	latch2();
	lcd_data_print2("WELl");
	latch2();
	_delay_ms(1000);
	lcd_clear();
}

// funtion to do buzzer
void buzzer()
{					  // setting up the buzzer
	DDRD |= (1 << 1); // setup DDR for the buzzer
	PORTD |= (1 << 1);
	_delay_ms(500);
	PORTD &= ~(1 << 1);
	_delay_ms(1800);
	stopBuzer();
}

void stopBuzer()
{
	DDRD &= ~(1 << 1); // stop the buzzer from ringing
	cli();			   // disable external interrupts globally
	//displayWelcome();  // Call the function to display the welcome message
	car_approaching();
}

void lcd_clear(){
	lcd_cmd(0x01); //clear screen
}

void lcd_print(char* data_print){
	int length=strlen(data_print);
	
	for (int i=0; i<length; i++){
		lcd_data(data_print[i]);
	}
}


void car_approaching(){
	
	lcd_init2();
	lcd_cmd2(0x80); // row 1
	latch2();
	lcd_data_print2("CAR APPROACHING");
	latch2();
	_delay_ms(2000);
	lcd_clear();
	registration();
	gate();
	
}

void registration(){
	lcd_init2();
	lcd_cmd2(0x80); // row 1
	latch2();
	lcd_data_print2("REGISTRATION IN PROGRESS.");
	latch2();
	_delay_ms(10000);
	lcd_clear();
}

void gate(){
	lcd_init2();
	lcd_cmd2(0x80); // row 1
	latch2();
	lcd_data_print2("GATE OPENING.");
	latch2();
	_delay_ms(3000);
	gateopen();
	lcd_clear();
	lcd_init2();
	lcd_cmd2(0x80); // row 1
	latch2();
	lcd_data_print2("CAR PASSING.");
	latch2();
	_delay_ms(3000);
	lcd_init2();
	lcd_cmd2(0x80); // row 1
	latch2();
	lcd_data_print2("GATE CLOSING.");
	latch2();
	_delay_ms(2000);
	lcd_clear();
	gateclose();
}

//------------------gate motors------------------------------
void gateopen(){
	PORTC |=(1<<0) | (1<<2);
	_delay_ms(3000);
	PORTC &=~(1<<0);
}

void gateclose(){
	PORTC &= ~(1<<0);
	PORTC |=(1<<0) | (1<<2);
	_delay_ms(2000);
	PORTC &=~(1<<0);
}


//*********************************************************************************************************************************************************************
// function for the servo motor
void servo(int rotations)
{
	DDRJ |= (1 << 0); // Set PJ0 as output pin
	PORTJ = 0x00;

	for (int i = 0; i < rotations; ++i)
	{

		// Rotate motor to 0 degree
		PORTJ |= (1 << 0);
		_delay_us(1000);
		PORTJ = 0x00;
		_delay_ms(1000);

		// Rotate motor to 180 degrees
		PORTJ |= (1 << 0);
		_delay_us(2000);
		PORTJ = 0x00;
		_delay_ms(1000);

		// Rotate motor to 360 degrees
		PORTJ |= (1 << 0);
		_delay_us(3000);
		PORTJ = 0x00;
		_delay_ms(1000);
	}
}

//*********************************************************************************************************************************************************************
// function for the UART
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 8UL))) - 1)
// Initialize UART for ATmega2560
void initUART()
{
	// Set baud rate to 9600 (for 16 MHz frequency)
	// Set baud rate to 9600 (for example)
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;

	// Enable transmitter and receiver
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);

	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

	// Debug message
	// sendString("UART Initialized at 9600 bps\r\n");
}

// Transmit a character via UART
void sendChar(char data)
{
	// Wait until buffer is empty
	while (!(UCSR0A & (1 << UDRE0)))
		;
	// Put data into buffer, sends the data
	UDR0 = data;
}

// Transmit a string via UART
void sendString(const char *str)
{
	// Loop through each character in the string and send it
	while (*str != '\0')
	{
		sendChar(*str);
		str++;
	}
}

void virtualTerminal()
{
	// Send the sentence over UART
	sendString("\r\n Operator\r\n\n");
	sendString("\r\n\n");
	displayMenuLogin();
	_delay_ms(1000); // Delay for 1 second
}

// *********************************************************************************************************************************************************************
// implement the Menu at the serial console for the attendant

// function to send serial Data
void sendSerialData(char data)
{
	while (!(UCSR0A & (1 << UDRE0)))
		;
	UDR0 = data;
}

// function to receive serial data
char receiveSerialData()
{
	while (!(UCSR0A & (1 << RXC0)))
		;
	return UDR0;
}
// function to display the menu to the attendant to login
void displayMenuLogin()
{
	//sendString("Please Enter your Password\r\n");
	sendString("Enter Password here: ");
}

// function to display the menu to the attendant
void displayMenu()
{
	sendString("\r\n\n");
	sendString("\r\n\n");
	sendString("Menu Options:\r\n");
	sendString("\r\n\n");
	sendString("1) Tourist registration\r\n");
	sendString("2) Check tourists count\r\n");
	sendString("3) Check vehicles in park\r\n");
	sendString("4) Check collected funds\r\n");
	sendString("5) Check drivers count\r\n");
	sendString("6) Check bottles in fridge\r\n");
	sendString("7) Replenish Fridge\r\n");
	sendString("8) Check cars waiting outside park\r\n");
	sendString("9) Check park status\r\n");
	sendString("0) Logout\r\n");

	sendString("\r\n");
	sendString("Enter your choice: ");
}

// function to capture the user input from the user
// Function to receive user input from UART
void receiveUserInput(char *inputBuffer, int bufferSize)
{
	int index = 0;
	char receivedChar;

	// Read characters until Enter is pressed or buffer is full
	while (1)
	{
		// Wait until data is available in the receive buffer
		while (!(UCSR0A & (1 << RXC0)))
			;

		// Read the received character
		receivedChar = UDR0;

		// Handle backspace
		if (receivedChar == '\b' && index > 0)
		{
			index--;
			inputBuffer[index] = '\0'; // Null-terminate the string at the backspace position
		}
		// Handle newline or carriage return
		else if (receivedChar == '\r' || receivedChar == '\n')
		{
			// Null-terminate the string and exit the loop
			inputBuffer[index] = '\0';
			break;
		}
		// Store the character in the inputBuffer array if there is space
		else if (index < bufferSize - 1)
		{
			inputBuffer[index++] = receivedChar;
			inputBuffer[index] = '\0'; // Null-terminate the string
		}

		// Echo the character back to the terminal
		sendChar(receivedChar);
	}
}

// function to check the action after the user input
void choiceChecker()
{

	// call the display menu function
	displayMenu();
	char userInput[20]; // Assuming a maximum of 20 characters for user input
	receiveUserInput(userInput, sizeof(userInput));

	int choice = atoi(userInput);

	// Call corresponding function based on user choice
	switch (choice)
	{
	case 1:
		// Implement additional menu option
		performRegistration();
		break;
	case 2:
		checkTouristsCount();
		break;
	case 3:
		checkVehiclesInPark();
		break;
	case 4:
		checkCollectedFunds();
		break;
	case 5:
		checkDriversCount();
		break;
	case 6:
		checkBottlesInFridge();
		break;
	case 7:
		replenishFridge();
		break;

	case 8:
		checkCarsWaiting();
		break;
	case 9:
		checkParkStatus();
		break;
	case 0:
		logout();
		break;
	default:
		sendString("\r\n");
		sendString("\r\nInvalid choice!!!\r\n");
		break;
	}
}

//*********************************************************************************************************************************************************************
// functions to handle the registration
void performRegistration()
{
	char carNumberPlate[6]; // Assuming a maximum of 6 characters for the car number plate
	char numBelow10[3];		// Assuming a maximum of 3 characters for tourists below 10 years old
	char numAbove10[3];		// Assuming a maximum of 3 characters for tourists above 10 years old

	sendString("\r\nEnter car number plate: ");
	receiveUserInput(carNumberPlate, sizeof(carNumberPlate));

	sendString("\r\nEnter number of tourists below 10 years old: ");
	receiveUserInput(numBelow10, sizeof(numBelow10));

	sendString("\r\nEnter number of tourists above 10 years old: ");
	receiveUserInput(numAbove10, sizeof(numAbove10));

	// Convert strings to integers if necessary
	int touristsBelow10 = atoi(numBelow10);
	int touristsAbove10 = atoi(numAbove10);

	// TODO: Implement code to update park data with the provided information

	// Display fee message
	sendString("\r\nAdult fee -----> 5,000 per person\r\n");
	sendString("\r\nAdult fee -----> 2,000 per person\r\n");

	// Display success message
	sendString("\r\nRegistration successful.\r\n");
	_delay_ms(1000); // Delay for 1 second

	// Return to the display menu
	displayMenu();
}

//*********************************************************************************************************************************************************************

// here we implement the functions for the attendant

void checkTouristsCount()
{
	// Implement code to check tourists count
	// Send the count information via UART
}

void checkVehiclesInPark()
{
	// Implement code to check vehicles in park
	// Send the count information via UART
}

void checkCollectedFunds()
{
	// Implement code to check collected funds
	// Send the count information via UART
}

void checkDriversCount()
{
	// Implement code to check drivers count
	// Send the count information via UART
}

void checkBottlesInFridge()
{
	// Implement code to check bottles in fridge
	// Send the count information via UART
}

void replenishFridge()
{
	// Implement code to replenish fridge
	// Send the count information via UART
}

void checkCarsWaiting()
{
	// Implement code to check cars waiting
	// Send the count information via UART
}

void checkParkStatus()
{
	// Implement code to check park status
	// Send the count information via UART
}
char userInput[20]; // Assuming a maximum of 20 characters for user input

void performLogin()
{
	receiveUserInput(userInput, sizeof(userInput));

	
    char read_data[4];  // Allocate space for the string

    // Read the string from EEPROM
	for (int i = 0; i < 3; i++)
	{
		read_data[i] = EEPROM_read(40 + i);
		_delay_ms(1000);
	}
    // Null-terminate the string
    read_data[3] = '\0';
	if (strcmp(userInput, read_data) == 0)
	{
		// Successful login, allow access to menu options
		choiceChecker();
	}
	else
	{
		// Invalid credentials, handle accordingly (e.g., display error message)
		sendString("\r\nInvalid input!!!\r\n ");
		sendString(read_data);
		displayMenuLogin();
	}
}

// function to logout the attendant
void logout()
{
	// take them to the fist screen
	virtualTerminal();
}

//*********************************************************************************************************************************************************************
// functions to set and unset flags to handle if conditions
int flag = 0;
void setFlag()
{
	flag = 1;
}

void unsetFlag()
{
	flag = 0;
}
// ********************************************INTERRUPT SERVICE ROUTINE FOR THE BUTTON********************************************************************************
// do the interrupt service run tine for INT0


int main(void)
{
	// Initialize UART
	initUART();

	// write data to EEPROM
	EEPROM_write(40, '1');
	_delay_ms(1000);
	EEPROM_write(41, '2');
	_delay_ms(1000);
	EEPROM_write(42, '3');

	_delay_ms(1000);
	// call the virtual terminal function
	virtualTerminal();
	
	while (1)
	{
		// set up the LCD and the keypad at the fridge
		DDRL = 0b00000111;
		DDRG = 0xff; // lcd output
		DDRH = 0xff; // lcd output

		// setup the LCD for the gate. //Keypad //this will be the gate and its functions will be having a 2 at the end
		DDRA = 0xfff;
		DDRB = 0b00000111;
		DDRC=0XFF;
		DDRK=0X00;
		// setting up the button and interrupt
		DDRD &= ~(1 << 0); // setting 0 for input of the button
		// for 1st btn
		EICRA |= (1 << INT0); // Set for falling edge triggering
		EIMSK |= (1 << INT0); // Enable INT0
		sei();				  // enable external interrupts globally

		// call the default gate message
		

		// Wait for user input and handle the choice
		performLogin();

		// call the capture drinks function

		captureDrinks();
	}
}





