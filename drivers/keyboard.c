#include "keyboard.h"
#include "serialports.h"

// 0x64 is the status register, the lowest bit is 0 if no data is ready to be read and is 1 if data is ready to be read
// 0x60 is the data register and gives the SCANCODE not ascii char code of the key


//need a translation table:
const char scancodeTranslationTable[] = {
    0, 27, '1' , '2', '3' , '4' ,'5', '6', '7' , '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y' ,'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j','k', 'l',';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b','n', 'm', ',', '.', '/', 0, '*', 0, ' ' 
};

//to check for a keypress,
//read from port 0x64
//if lowest bit is set to 1 then there is a key to read
//check 0x60 for the scancode
// rn ignoring breakcodes just to see something work without it taking 3000x longer (not rly i just dont want to deal with a keyboard keys currently pressed buffer thing yet), so we are ignoring anything where 0x80 is set

char getPressedASCII(){
    char statusreg = inb(0x64);
    if ((statusreg & 1 ) == 0){ //check to see if the lowest bit is enabled
        return 0; //return null if there is nothing new to get as of right now
    }

    unsigned char scancode = inb(0x60); //its kinda nice that char doesnt have to be a character, just any 8bit number

    if (scancode & 0x80){ //ignoring release bit for now cause i dont want to make a buffer for currently down keys yet lol
        return 0;
    } //comment this if statement out whne adding key up events

    if (scancode < sizeof(scancodeTranslationTable)){
        return scancodeTranslationTable[scancode];
    }


    //fallback
    return 0;

}
