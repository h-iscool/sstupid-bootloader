#ifndef SERIALPORTS_H
#define SERIALPORTS_H

static inline unsigned char inb(unsigned short port){
    unsigned char result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port)); // ATT SYNTAX: inb(read input byte)
    return result;
}

//__asm__ volatile ( "code" : outputs : inputs : clobbers );

static inline void outb(unsigned short port, unsigned char data){
    __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

#endif