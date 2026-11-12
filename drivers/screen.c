#include "vga.h"
#include "vbe.h"
#include "screen.h"


static int videoDriver = 0;

static char vga_colorScheme = 0x0f;

void printString(const char* string) {
	switch(videoDriver){
		case DRIVER_VGA:
			vga_printString(string, vga_colorScheme);
			break;
		case DRIVER_VBE:
			break;
	}

}

void printChar(char toPrint) {
	switch(videoDriver){
		case DRIVER_VGA:
			vga_printChar(toPrint, vga_colorScheme);
			break;
		case DRIVER_VBE:
			break;
	}
}

void clearScreen() {
	switch(videoDriver){
		case DRIVER_VGA:
			char vpa_colorScheme = vga_pack_colors(VGA_COLOR_BLACK,VGA_COLOR_BLUE); 
			vga_clearScreen(vga_colorScheme);
			break;
		case DRIVER_VBE:
			break;
	}
}

void initScreen(int screen){
	videoDriver = screen;
	switch(videoDriver){
		case DRIVER_VGA:
			vga_clearScreen(vga_colorScheme);
			break;
		case DRIVER_VBE:
			//vbe_debug();
			break;
	}

}
