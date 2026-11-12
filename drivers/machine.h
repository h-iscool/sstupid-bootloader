#include <stdint.h>
#ifndef MACHINE_H
#define MACHINE_H

// void time_init(); // CURRENTLY CALLED IN ASM SO NO NEED TO EXPORT

// void timer_tick_call();

void sleep(uint32_t ms);

#endif