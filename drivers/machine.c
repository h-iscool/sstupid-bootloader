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

volatile uint32_t timer_ticks = 0;

void timer_tick_call(){
    timer_ticks++;

    //send end of interrupt to the programable interrupt controler
    
}


void sleep(uint32_t ms){
    uint32_t start_time = timer_ticks;

    while ((timer_ticks-start_time)<ms){
        __asm__ __volatile__ ("hlt");
    }
}