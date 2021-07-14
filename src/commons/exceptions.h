#ifndef EXCEPTIONS_H 
#define EXCEPTIONS_H

// #include <exception> 

#include "defs.h"

class Exception {
    string message;

    public:
        Exception() {}

        Exception(string msg): message(msg) {};

        string what();
};

class IllegalArgumentException : public virtual Exception {
    // the message associated with the exception
    public:
        IllegalArgumentException(string msg): Exception(msg) {};
};

class ScanningError: public virtual Exception {
    public:
        ScanningError(string msg): Exception(msg) {};
};

#endif