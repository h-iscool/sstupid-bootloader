#include "./vbe_new.h"
#include "../drivers/machine.h"

void LOAD_TESTING(){ //ikik its c not asm but C is so much more readable. also we should try to keep this file kinda clean... maybe

    bios_print_vga_char_int_test('B');bios_print_vga_char_int_test('I');bios_print_vga_char_int_test('O');bios_print_vga_char_int_test('S');bios_print_vga_char_int_test(' ');bios_print_vga_char_int_test('r');bios_print_vga_char_int_test('e');bios_print_vga_char_int_test('a');bios_print_vga_char_int_test('l');bios_print_vga_char_int_test('-');bios_print_vga_char_int_test('m');bios_print_vga_char_int_test('o');bios_print_vga_char_int_test('d');bios_print_vga_char_int_test('e');bios_print_vga_char_int_test(' ');bios_print_vga_char_int_test('i');bios_print_vga_char_int_test('n');bios_print_vga_char_int_test('t');bios_print_vga_char_int_test('s');
    
    check_vbe_support();

    

    while (1){ //comment to continue, uncomment to hlt before loading the console
        __asm__ ("hlt");
    }

}