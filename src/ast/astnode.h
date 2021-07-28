#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>
#include <vector>

#include "../commons/defs.h"
#include "../scanner/token.h"
// #include "vardeclnode.h"

class ASTNode {
    public:
        virtual void print(uint32_t depth, std::ostream& printTo) = 0;

        virtual operator string() = 0;

        // virtual bool checkSemantic() = 0;

        // using  VecIterator = std::vector<ASTNode*>::iterator;
        template <typename T>
        void printVector(T begin, T end, uint32_t depth, std::ostream& printTo);

        template <typename T>
        void deleteVectorMembers(T begin, T end);

        template <typename T>
        string toString(T begin, T end);

        void addVar(ASTNode *var) {};
};

#endif 