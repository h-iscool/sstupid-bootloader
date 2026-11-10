#include <stdbool.h>
#include "CPP_VGA_TEST_1.hpp"

static volatile char * VGA_MEMORY = (volatile char *)0xb8000;

// class VGA
// {
// private:
//     /* data */

//     VGA(/* args */);
//     ~VGA();

//     VGA(const VGA&) = delete; // removes the copy constructor
//     VGA& operator=(const VGA&) = delete; //removes the copy assignment constructor

//     int cursor_x = 0;
//     int cursor_y = 0;

    

//     const char color = 0x0f;


// public:


//     static VGA& getInstance(){
//         static VGA instance; // only gets defined once
//         return instance; //then always returned here
//     } // i spent way to long making that work for technically no real reason but i wanted to be kinda safe, actually idk if it even is more safe cause all of ts junk

//     void printChar(const char character);
//     void printString(const char * string);

// };

VGA::VGA()
{

}

VGA::~VGA()
{
}

void VGA::printChar(const char character){

    int offset = (cursor_y * 80 + cursor_x)*2;

    VGA_MEMORY[offset] = character;
    VGA_MEMORY[offset+1] = color;

    cursor_x++;

    if (cursor_x == 80){
        cursor_x = 0;
        cursor_y++;
    }


}

void VGA::printString(const char * string){
    for (int i = 0; string[i] != '\0'; i++){
        printChar(string[i]);
    }
}

