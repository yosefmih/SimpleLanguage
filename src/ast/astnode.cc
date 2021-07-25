#include "astnode.h"

template <typename T>
void ASTNode::printVector(std::vector<T*>& vec, uint32_t depth, std::ostream& printTo) {
    auto vecIterator = vec.begin();
    while (vecIterator != vec.end()) {
        (*vecIterator)->print(depth + 1, printTo);
        vecIterator++; 
    }
}

template <typename T>
string ASTNode::toString(std::vector<T*>& vec) {
    string str;
    auto vecIterator = vec.begin();
    while (vecIterator != vec.end()) {
        str += string(*vecIterator);
        str += "\n"
        vecIterator++;
    }
    return string;
}