// void printChar(char chartoprint, int offset){
//     char* screen = (char*)0xb8000;
//     screen[0+(offset*2)] = chartoprint;
//     screen[1+(offset*2)] = 0x0F;
// }

#include "vga_funcs.h"



static int vga_cursor_x = 0;
static int vga_cursor_y = 0;

void vga_clearScreen(char color){
    volatile char* screen = (volatile char*)0xb8000;

    for (int i = 0; i<4000; i+=2){
        screen[i] = ' '; //empty char
        screen[i+1] = color; //set color
    }
}

void vga_printChar(char toPrint, char color){ //https://wiki.osdev.org/Text_UI

    if (toPrint == '\n'){
        vga_cursor_x = 0;
        vga_cursor_y++;
    }
    else{
        volatile char * screen = (volatile char*)0xb8000;
        int offset = (vga_cursor_y*80+vga_cursor_x)*2;

        screen[offset] = toPrint;
        screen[offset+1] = color; // no color rn
        vga_cursor_x++;
    }

    if (vga_cursor_x>=80){
        vga_cursor_x = 0;
        vga_cursor_y++;
    }
    

}

void vga_printString(const char* string, char color){
    for (int i = 0; string[i] != '\0'; i++){
        vga_printChar(string[i], color);
    }
}

char vga_pack_colors(char characterColor, char backgroundColor){
    return (backgroundColor << 4) | (characterColor); // first 4 bits = background so bitshift the background 4 left to make room to add the char color which is the last 4 bits. the best way to combine 2 sets of 4 bits like this is to or them as long as there is no chance of upper/lower bit data that would corrupt the value
}