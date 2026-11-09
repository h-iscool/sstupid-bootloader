#include <stddef.h>

void *memset(void * ptr, size_t val, size_t size){
    
    unsigned char * p = (unsigned char *)ptr; //cant index a void pointer so we index an unsigned char ptr, why unsigned? because for some reason there can be a negitive char

    for (size_t i = 0; i<size; i++){
        p[i] = val;
    }

    return ptr;
}

void *memcpy(void * dest, const void * src, size_t size){

    unsigned char * d = (unsigned char *)dest;
    const unsigned char * s = (const unsigned char *)src;

    for (size_t i = 0; i<size; i++){
        d[i] = s[i];
    }

    return dest;
}

void *memcpy32(void * dest, const void * src, size_t size){ // pretty much only to copy 4byte things like pixels

    uint32_t * d = (uint32_t *)dest;
    const uint32_t * s = (const uint32_t *)src;

    for (size_t i = 0; i<size; i++){
        d[i] = s[i];
    }

    return dest;
}

size_t strlen(const char * string){

    size_t returnval = 0;

    for (size_t i = 0; string[i] != '\0'; i++){
        returnval++;
    }

    return returnval;

}

// bool strcmp (const char * string_A, const char * string_B){ //normal strcmp returns the diff of char code this returns bool, might change later
//     size_t stringLen = strlen(string_A);
//     if (!(strlen(string_A) == strlen(string_B))){
//         return false;
//     }

//     for (size_t i = 0; i<stringLen; i++){
//         if (!(string_A[i] == string_B[i])){ return false;}
//     }

//     return true;

// }

int strcmp (const char * string_A, const char * string_B){

    size_t i = 0;

    while (string_A[i] == string_B[i]){
        if (string_A[i] == '\0'){
            return 0; //because they both are equal and both end
        }
        i++;
    }

    return (unsigned char)string_A[i] - (unsigned char )string_B[i];

}

char * itoa (int value, char * strbuff, int base){

    if (base < 2 || base > 36){ //no crazy high bases or under b2
        strbuff[0] = '\0';
        return strbuff;
    }

    char * ptr = strbuff;
    

    if ((value < 0) && (base == 10)){
        *ptr = '-';
        ptr++;
    }

    char* low = ptr;
    int num = value;

    do{
        int remainder = num % base;
        if (remainder < 0){
            remainder=-remainder; // needs to be positive for the lookup
        }

        *ptr = "0123456789abcdefghijklmnopqrstuvwxyz"[remainder];
        ptr++;

        num /= base;
    }while (num != 0); //do while for at least 1 iteration;

    *ptr = '\0'; //null terminated;

    char * high = ptr -1;
    while (low < high){
        char temp = *low;
        *low = *high;
        *high = temp;
        low++;
        high--;
    }

    return strbuff;

}


//todo:
//format string fn, given args and a string, parse and format it like print f
//but it shoulrd be here cause it should just parse the string not print it
