#include "common.h"

#include <cstdint> 
#include <sstream>

void toLower(string str) {
    for (uint32_t i=0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
}


string addIndent(string str, uint32_t depth) {
    string result;
    string indentation;
    for (int i = 0; i < depth; i++) {
        indentation += TAB_SPACES;
    }

    string delimiter = "\n";
    size_t pos = 0; 
    while ((pos = str.find(delimiter)) != str.npos) {
        result += indentation + str.substr(0, pos) + "\n";
        // +1 because the delimiter, \n, is size 1
        str.erase(0, pos + 1);
    }

    return result;
}

string indentationAtDepth(uint32_t depth) {
    string indentation;
    for (int i = 0; i < depth; i++) {
        indentation += TAB_SPACES;
    }
    return indentation;
}

// template <typename T, typename A>
// void deleteVectorMembers(std::vector<T*, A>& vec) {
//     auto vecIterator = vec.begin();
//     while (vecIterator != vec.end()) {
//         delete *vecIterator;
//         vecIterator++;
//     }
// }

uint32_t stringToInt(string str) {
    std::stringstream strStream(str);
    uint32_t res = 0;
    strStream >> res;
    return res;
}