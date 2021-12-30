#include <iostream>
#define LOGGING 1
#if LOGGING
// LOGGING configuration macros
#define DISPLAY_TIME 1
    // cpprest provides macros for all streams but std::clog in basic_types.h
    #ifdef _UTF16_STRINGS
    // On Windows, all strings are wide
    #define uclog std::wclog
    #else
    // On POSIX platforms, all strings are narrow
    #define uclog std::clog
    #endif // endif _UTF16_STRINGS

    #if DISPLAY_TIME
    #define LOG(msg) uclog << "[" << __TIME__ << "] " << msg << std::endl
    #else
    #define LOG(msg) uclog << msg << std::endl;
    #endif
#endif