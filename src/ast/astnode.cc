#include "astnode.h"

template <typename T>
void ASTNode::printVector(T begin, T end, uint32_t depth, std::ostream& printTo) {
    T vecIterator = begin;
    while (vecIterator != end) {
        (*vecIterator)->print(depth + 1, printTo);
        vecIterator++; 
    }
}

template <typename T>
string ASTNode::toString(T begin, T end) {
    string str;
    T vecIterator = begin;
    while (vecIterator != end) {
        str += string(**vecIterator);
        str += "\n";
        vecIterator++;
    }
    return str;
}

template <typename T>
void ASTNode::deleteVectorMembers(T begin, T end) {
    T vecIterator = begin;
    while (vecIterator != end) {
        delete *vecIterator;
        vecIterator++;
    }
}