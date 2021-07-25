#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>
#include <vector>

#include "../commons/defs.h"
#include "../scanner/token.h"

class ASTNode {
    public:
        virtual void print(uint32_t depth, std::ostream& printTo) = 0;

        virtual operator string() = 0;

        virtual bool checkSemantic() = 0;

        template <typename T>
        void printVector(std::vector<T*>& vec, uint32_t depth, std::ostream& printTo);

        template <typename T>
        string toString(std::vector<T*>& vec);
};

#endif 