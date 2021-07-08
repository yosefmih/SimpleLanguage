#include "exceptions.h"
#include "defs.h"

string Exception::what() {
    return message;
}