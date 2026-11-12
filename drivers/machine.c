#include "./machine.h"
#include "./serialports.h"
#include <stdint.h>

void time_init(){

    int frequency = 1000; // rn just making it static. maybe need to change later but tracking ms is good enough rn

    uint32_t timeDevisor = 1193180 / frequency; // goofy ass # but its a constant on all devices that use 

    outb(0x43, 0x36); //sends command byte to PIT channel port (0x43)

    uint8_t lobyte = (uint8_t)(timeDevisor& 0xff);
    uint8_t hibyte = (uint8_t)((timeDevisor>>8)&0xff);
    
    //send new freq div to data port of PIT
    outb(0x40, lobyte);
    outb(0x40, hibyte);

}

void int_remaps(){
    unsigned char a1 = inb(0x21);
    unsigned char a2 = inb(0xa1);

    outb(0x20,0x11);
    outb(0xa0,0x11);

    outb(0x21,0x20);
    outb(0xa1,0x28);

    outb(0x21,0x04);
    outb(0xa1,0x02);

    outb(0x21,0x01);
    outb(0xa1,0x01);

    outb(0x21,a1);
    outb(0xa1,a2);
}

volatile uint32_t timer_ticks = 0;

void timer_tick_call(){
    timer_ticks++;

    //send end of interrupt to the programable interrupt controler
    outb(0x20,0x20);
}


void sleep(uint32_t ms){
    uint32_t start_time = timer_ticks;

    while ((timer_ticks-start_time)<ms){
        __asm__ __volatile__ ("hlt");
    }
}