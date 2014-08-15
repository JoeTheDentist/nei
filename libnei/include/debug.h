#ifndef DEBUG_H
#define DEBUG_H

//#define DEBUG

#ifdef DEBUG
    #include <iostream>
    #define TRACE(trace) std::cout << trace << std::endl
#else
    #define TRACE(trace)
#endif

#endif // DEBUG_H
