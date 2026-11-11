#ifndef CPP_C_INTEG_TEST_H
#define CPP_C_INTEG_TEST_H


#ifdef __cplusplus // only for if its cpp to tell cpp that this is for c lmao ts is complex
extern "C"{
#endif


    void load_cpp_test(); // all code that even *touches* c or asm gets put here, if theres a chance it will on accidnet its better to put it here


#ifdef __cplusplus
}
#endif

// #ifdef __cplusplus
//     extern "C" int atexit(void (*func)(void)){
//         return 0;
//     }
// #endif

#endif