#include "../drivers/vbe.h"
#include "../drivers/machine.h"
#include "../drivers/screen.h"

void LOAD_TESTING(){ //ikik its c not asm but C is so much more readable. also we should try to keep this file kinda clean... maybe

    printString("Loading VBE");

    sleep(1000);

    check_vbe_support(); // need to make init fn
    vbe_scan_resolutions();
    vbe_try_set_resolution(1920,1080);
    vbe_fill_screen(0xff555577);
    


    // while (1){ //comment to continue, uncomment to hlt before loading the console
    //     __asm__ ("hlt");
    // }

}