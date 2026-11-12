#include "keyboard.h"
#include "serialports.h"
#include <stdbool.h>

// 0x64 is the status register, the lowest bit is 0 if no data is ready to be read and is 1 if data is ready to be read
// 0x60 is the data register and gives the SCANCODE not ascii char code of the key


// //need a translation table:
// const char scancodeTranslationTable[] = {
//     0, 27, '1' , '2', '3' , '4' ,'5', '6', '7' , '8', '9', '0', '-', '=', '\b',
//     '\t', 'q', 'w', 'e', 'r', 't', 'y' ,'u', 'i', 'o', 'p', '[', ']', '\n', 0,
//     'a', 's', 'd', 'f', 'g', 'h', 'j','k', 'l',';', '\'', '`', 0, '\\',
//     'z', 'x', 'c', 'v', 'b','n', 'm', ',', '.', '/', 0, '*', 0, ' ' 
// };

// //to check for a keypress,
// //read from port 0x64
// //if lowest bit is set to 1 then there is a key to read
// //check 0x60 for the scancode
// // rn ignoring breakcodes just to see something work without it taking 3000x longer (not rly i just dont want to deal with a keyboard keys currently pressed buffer thing yet), so we are ignoring anything where 0x80 is set

// char getPressedASCII(){
//     char statusreg = inb(0x64);
//     if ((statusreg & 1 ) == 0){ //check to see if the lowest bit is enabled
//         return 0; //return null if there is nothing new to get as of right now
//     }

//     unsigned char scancode = inb(0x60); //its kinda nice that char doesnt have to be a character, just any 8bit number

//     if (scancode & 0x80){ //ignoring release bit for now cause i dont want to make a buffer for currently down keys yet lol
//         return 0;
//     } //comment this if statement out whne adding key up events

//     if (scancode < sizeof(scancodeTranslationTable)){
//         return scancodeTranslationTable[scancode];
//     }


//     //fallback
//     return 0;

// }


//NEW KEYBOARD STUFF
// sadly we need better translation tables too so ill just do that now too ig ;w;

const char scancodeToASCII[128] = { //https://users.utcluj.ro/~baruch/sie/labor/PS2/Scan_Codes_Set_1.htm
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' ,'-', '=', '\b',
    '\t', 'q', 'w' ,'e', 'r', 't', 'y' ,'u', 'i' ,'o' ,'p', '[',']', '\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',

    0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,
    '7','8','9','-',
    '4','5','6','+',
    '1','2','3',
    '0','.',0,0,0,0,0
};

const char shiftedScancodeToASCII[128] = { //https://users.utcluj.ro/~baruch/sie/labor/PS2/Scan_Codes_Set_2.htm
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')' ,'_', '+', '\b',
    '\t', 'Q', 'W' ,'E', 'R', 'T', 'Y' ,'U', 'I' ,'O' ,'P', '{','}', '\n',0,
    'A','S','D','F','G','H','J','K','L',':','"','~',0,'|',
    'Z','X','C','V','B','N','M','<','>','?',

    0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,
    '7','8','9','-',
    '4','5','6','+',
    '1','2','3',
    '0','.',0,0,0,0,0
};

bool keys_down[128] = {false};

char keyboardbuffer[256]; // cause we now need to store the keys
int bufferHead = 0;
int bufferTail = 0;

void keyboard_int_handler(){
    unsigned char scancode = inb(0x60); //get the scan code

    bool isRelease = ((scancode & 0x80) != 0);
    unsigned char strippedScanCode = scancode & 0x7f; //so its easier to index

    keys_down[strippedScanCode] = !isRelease; // set that key to be pressed

    if (!isRelease){
        char asciiChar = 0;

        if (keys_down[0x2a] || keys_down[0x36]){ //left and right shifts
            if (strippedScanCode < sizeof(shiftedScancodeToASCII)){
                asciiChar = shiftedScancodeToASCII[strippedScanCode];
            }
        }else{
            if (strippedScanCode < sizeof(shiftedScancodeToASCII)){
                asciiChar = scancodeToASCII[strippedScanCode];
            }
        }
        
        if (asciiChar != 0 ){
            keyboardbuffer[bufferHead] = asciiChar;
            bufferHead = (bufferHead+1)%256; //ring buffer push
        }

    }
    outb(0x20,0x20); //ack the int
}



//now remake the getpressedascii with this bs

char getPressedASCII(){
    if (bufferHead == bufferTail){
        return 0; //because no change in the keys
    }

    char character = keyboardbuffer[bufferTail]; // get oldest char in the buffer that HASNT been read yet

    bufferTail = (bufferTail+1)%256; //mod to wrap arround
    return character;
}