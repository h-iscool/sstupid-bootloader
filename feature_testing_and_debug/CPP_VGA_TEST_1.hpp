#ifndef CPP_VGA_TEST_1_H
#define CPP_VGA_TEST_1_H

//#ifndef __cplusplus = do this if gcc is compiling
//#ifdef __cplusplus = do this if g++ is compiling

class VGA
{
private:
    /* data */

    VGA(/* args */);
    ~VGA();

    VGA(const VGA&) = delete; // removes the copy constructor
    VGA& operator=(const VGA&) = delete; //removes the copy assignment constructor

    int cursor_x = 0;
    int cursor_y = 0;

    

    const char color = 0x0f;


public:


    static VGA& getInstance(){
        static VGA instance; // only gets defined once
        return instance; //then always returned here
    } // i spent way to long making that work for technically no real reason but i wanted to be kinda safe, actually idk if it even is more safe cause all of ts junk

    
    void printChar(const char character);
    void printString(const char * string);

};

#endif