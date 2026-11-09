#ifndef SCREEN_H
#define SCREEN_H

#define DRIVER_VGA 1
#define DRIVER_VBE 2

void printString(const char* string);
void printChar(char toPrint);
void clearScreen();
void initScreen(int screen);

#endif
