#include "./CPP_VGA_TEST_1.hpp"

extern "C" void load_cpp_test(){
    VGA& vga_screen_instance = VGA::getInstance();

    vga_screen_instance.printString("WORKS!!!!!!");

}