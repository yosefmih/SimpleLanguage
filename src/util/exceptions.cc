#include "exceptions.h"
#include "common.h"

string Exception::what() {
    return message;
}