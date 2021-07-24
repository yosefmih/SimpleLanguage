#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>

#include "../commons/defs.h"
#include "../scanner/token.h"

class ASTNode {
    public:
        
        virtual void print(uint32_t depth, std::ostream& printTo) = 0;

        virtual operator string() const = 0;

        virtual bool checkSemantic() = 0;
};

#endif 