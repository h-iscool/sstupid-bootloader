void printChar(char chartoprint, int offset){
    char* screen = (char*)0xb8000;
    screen[0+(offset*2)] = chartoprint;
    screen[1+(offset*2)] = 0x0F;
}

void clearScreen(){
    volatile char* screen = (volatile char*)0xb8000;

    for (int i = 0; i<4000; i+=2){
        screen[i] = ' '; //empty char
        screen[i+1] = 0x0F; //clear colors
    }
}

void kernel_main() {

    clearScreen();

    volatile char* screen = (volatile char*)0xb8000;

    screen[0] = 'L'; screen[1]=0x0f;
    screen[2] = 'o'; screen[3]=0x0f;
    screen[4] = 'a'; screen[5]=0x0f;
    screen[6] = 'd'; screen[7]=0x0f;
    screen[8] = 'e'; screen[9]=0x0f;
    screen[10] = 'd'; screen[11]=0x0f;
    screen[12] = ' '; screen[13]=0x0f;
    screen[14] = 'c'; screen[15]=0x0f;
    screen[16] = '!'; screen[17]=0x0f;
    
    while(1) {
        __asm__("hlt");
    }
}
