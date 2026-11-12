#include <stdint.h>
#include "../../../drivers/screen.h"
#include "../../../drivers/string.h"

typedef struct 
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code; 
    uint32_t eip, cs, eflags, useresp, ss;
} registers_struct;

const char *exception_messages[32] = {
    "Divide By Zero Exception",
    "Debug Exception",
    "Non Maskable Interrupt Exception",
    "Breakpoint Exception",
    "Into Detected Overflow Exception",
    "Out of Bounds Exception",
    "Invalid Opcode Exception",
    "No Coprocessor Exception",
    "Double Fault Exception",
    "Coprocessor Segment Overrun Exception",
    "Bad TSS Exception",
    "Segment Not Present Exception",
    "Stack Fault Exception",
    "General Protection Fault Exception",
    "Page Fault Exception",
    "Unknown Interrupt Exception (Reserved)",
    "Coprocessor Fault (x87 Floating-Point) Exception",
    "Alignment Check Exception",
    "Machine Check Exception",
    "SIMD Floating-Point Exception",
    "Virtualization Exception", 
    "Control Protection Exception", 
    "Reserved", 
    "Reserved",
    "Reserved", 
    "Reserved", 
    "Reserved", 
    "Reserved", 
    "Hypervisor Injection Exception",
    "VMM Communication Exception", 
    "Security Exception", 
    "Reserved"
};

void _fault_handler(registers_struct * regs){
    if (regs->int_no<32){
        clearScreen();
        printString("CPU EXCEPTION: ");
        printString(exception_messages[regs->int_no]);
        printString("\nEXCEPTION CODE: ");
        char itoastringbuffer[32];
        itoa(regs->err_code,itoastringbuffer,10); //large errs might print as neg but this is first 31 so its fine (probably)
        printString(itoastringbuffer);
        printString("\n");
        for(;;); //hlts sys
        while (1)
        {
            __asm__("hlt");
        }
        
    }
}