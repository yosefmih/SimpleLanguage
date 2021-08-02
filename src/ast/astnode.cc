#include "astnode.h"
#include "../commons/exceptions.h"

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

bool checkSemantic() {
    throw Exception("checkSemantic shouldn't be called on this node");
}

void ASTNode::buildEnv(SymbolTable *env) {
    throw Exception("buildEnv shouldn't be called on this node");
}

SymbolTable *ASTNode::getEnv() {
    return immediateEnv;
}
