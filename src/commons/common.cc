#include "common.h"

#include <cstdint> 

void toLower(string str) {
    for (uint32_t i=0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
}