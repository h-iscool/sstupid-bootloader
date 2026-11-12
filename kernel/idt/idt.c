#include "./idt.h"

#include <stdint.h>

typedef struct
{
    uint16_t func_ptr_lo;
    uint16_t kernel_seg_sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t func_ptr_hi;
} __attribute__((packed)) idt_entry;

typedef struct{
    uint16_t limit; //IDT size - 1
    uint32_t addr; // addr of IDT
} __attribute__((packed)) idt_loader;


idt_entry idt[256]; // x86 allows 256 idt entrys
idt_loader idt_reg;

void load_idt_handlers(void);
void load_isrs(void);

void set_idt_gate(int idtIndex, uint32_t handler_addr, uint16_t segment, uint8_t flags){
    idt[idtIndex].func_ptr_lo = (uint16_t)(handler_addr & 0xFFFF);
    idt[idtIndex].func_ptr_hi = (uint16_t)((handler_addr>>16)&0xffff);
    idt[idtIndex].kernel_seg_sel = segment;
    idt[idtIndex].always0 = 0;
    idt[idtIndex].flags = flags;
}

extern void load_idt(uint32_t idt_ptr); // this is defined in idt.asm

void idt_init(){
    //setup for loading idt:
    idt_reg.limit = (sizeof(idt_entry)*256)-1;
    idt_reg.addr = (uint32_t)&idt;

    for (int i = 0; i<256; i++){ //just to be safe 0 it all out
        set_idt_gate(i,0,0,0);
    }

    load_idt_handlers(); // just put them in here for code cleanlyness

    load_idt((uint32_t)&idt_reg);

}

extern void _isr32();

void load_idt_handlers(){

    load_isrs(); // index 0-31

    
    set_idt_gate(32,(uint32_t)_isr32,0x08, 0x8e); // BC of remap, idt 32-47 are also taken up. as of rn, only 32 has functionality
    //https://en.wikipedia.org/wiki/Interrupt_request

    
}




extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();
void load_isrs(){
    set_idt_gate(0,(unsigned)_isr0,0x08,0x8E);
    set_idt_gate(1,(unsigned)_isr1,0x08,0x8E);
    set_idt_gate(2,(unsigned)_isr2,0x08,0x8E);
    set_idt_gate(3,(unsigned)_isr3,0x08,0x8E);
    set_idt_gate(4,(unsigned)_isr4,0x08,0x8E);
    set_idt_gate(5,(unsigned)_isr5,0x08,0x8E);
    set_idt_gate(6,(unsigned)_isr6,0x08,0x8E);
    set_idt_gate(7,(unsigned)_isr7,0x08,0x8E);
    set_idt_gate(8,(unsigned)_isr8,0x08,0x8E);
    set_idt_gate(9,(unsigned)_isr9,0x08,0x8E);
    set_idt_gate(10,(unsigned)_isr10,0x08,0x8E);
    set_idt_gate(11,(unsigned)_isr11,0x08,0x8E);
    set_idt_gate(12,(unsigned)_isr12,0x08,0x8E);
    set_idt_gate(13,(unsigned)_isr13,0x08,0x8E);
    set_idt_gate(14,(unsigned)_isr14,0x08,0x8E);
    set_idt_gate(15,(unsigned)_isr15,0x08,0x8E);
    set_idt_gate(16,(unsigned)_isr16,0x08,0x8E);
    set_idt_gate(17,(unsigned)_isr17,0x08,0x8E);
    set_idt_gate(18,(unsigned)_isr18,0x08,0x8E);
    set_idt_gate(19,(unsigned)_isr19,0x08,0x8E);
    set_idt_gate(20,(unsigned)_isr20,0x08,0x8E);
    set_idt_gate(21,(unsigned)_isr21,0x08,0x8E);
    set_idt_gate(22,(unsigned)_isr22,0x08,0x8E);
    set_idt_gate(23,(unsigned)_isr23,0x08,0x8E);
    set_idt_gate(24,(unsigned)_isr24,0x08,0x8E);
    set_idt_gate(25,(unsigned)_isr25,0x08,0x8E);
    set_idt_gate(26,(unsigned)_isr26,0x08,0x8E);
    set_idt_gate(27,(unsigned)_isr27,0x08,0x8E);
    set_idt_gate(28,(unsigned)_isr28,0x08,0x8E);
    set_idt_gate(29,(unsigned)_isr29,0x08,0x8E);
    set_idt_gate(30,(unsigned)_isr30,0x08,0x8E);
    set_idt_gate(31,(unsigned)_isr31,0x08,0x8E);
}