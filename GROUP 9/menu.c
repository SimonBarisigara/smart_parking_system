#include "menu.h"

extern char userInput[20];

void virtualTerminal(void) {
    sendString("\r\n Operator\r\n\n");
    sendString("\r\n\n");
    displayMenuLogin();
    _delay_ms(1000);
}

void displayMenuLogin(void) {
    sendString("Enter Password here: ");
}

void displayMenu(void) {
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

void choiceChecker(void) {
    displayMenu();
    receiveUserInput(userInput, sizeof(userInput));
    int choice = atoi(userInput);
    switch (choice) {
        case 1: performRegistration(); break;
        case 2: checkTouristsCount(); break;
        case 3: checkVehiclesInPark(); break;
        case 4: checkCollectedFunds(); break;
        case 5: checkDriversCount(); break;
        case 6: checkBottlesInFridge(); break;
        case 7: replenishFridge(); break;
        case 8: checkCarsWaiting(); break;
        case 9: checkParkStatus(); break;
        case 0: logout(); break;
        default: sendString("\r\nInvalid choice!!!\r\n"); break;
    }
}

void performRegistration(void) {
    char carNumberPlate[6];
    char numBelow10[3];
    char numAbove10[3];
    sendString("\r\nEnter car number plate: ");
    receiveUserInput(carNumberPlate, sizeof(carNumberPlate));
    sendString("\r\nEnter number of tourists below 10 years old: ");
    receiveUserInput(numBelow10, sizeof(numBelow10));
    sendString("\r\nEnter number of tourists above 10 years old: ");
    receiveUserInput(numAbove10, sizeof(numAbove10));
    int touristsBelow10 = atoi(numBelow10);
    int touristsAbove10 = atoi(numAbove10);
    sendString("\r\nAdult fee -----> 5,000 per person\r\n");
    sendString("\r\nAdult fee -----> 2,000 per person\r\n");
    sendString("\r\nRegistration successful.\r\n");
    _delay_ms(1000);
    displayMenu();
}

void checkTouristsCount(void) {
}

void checkVehiclesInPark(void) {
}

void checkCollectedFunds(void) {
}

void checkDriversCount(void) {
}

void checkBottlesInFridge(void) {
}

void replenishFridge(void) {
}

void checkCarsWaiting(void) {
}

void checkParkStatus(void) {
}

char userInput[20];

void performLogin(void) {
    receiveUserInput(userInput, sizeof(userInput));
    char read_data[4];
    for (int i = 0; i < 3; i++) {
        read_data[i] = EEPROM_read(40 + i);
        _delay_ms(1000);
    }
    read_data[3] = '\0';
    if (strcmp(userInput, read_data) == 0) {
        choiceChecker();
    } else {
        sendString("\r\nInvalid input!!!\r\n ");
        sendString(read_data);
        displayMenuLogin();
    }
}

void logout(void) {
    virtualTerminal();
}