#include "../drivers/vbe.h"
#include "../drivers/machine.h"
#include "../drivers/screen.h"
#include "./vbeconsole.h"

void LOAD_TESTING(){ //ikik its c not asm but C is so much more readable. also we should try to keep this file kinda clean... maybe

    printString("Loading VBE");

    sleep(1000);

    init_vbeConsole(1920,1080,0xff555577);
    


    // while (1){ //comment to continue, uncomment to hlt before loading the console
    //     __asm__ ("hlt");
    // }

}