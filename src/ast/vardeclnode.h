#ifndef VARIABLE_DECL_NODE_H
#define VARIABLE_DECL_NODE_H

#include <iostream>

#include "astnode.h"
#include "literlanode.h"
#include "../commons/defs.h"

class VarDeclNode : public ASTNode {
    LitNode *identifier;
    // this denotes the size when the variable is an array
    // it should be set to -1 if the variable is not an array 
    int size;     
    
    public:
        VarDeclNode(LitNode *identifier, int size) : identifier(identifier), size(size) {}

        VarDeclNode(LitNode *identifier) : identifier(identifier), size(-1) {} 

        ~VarDeclNode();

        LitNode *getIdentifier();

        int getSize();

        bool isArray();

        void setIdentifier(LitNode *identifier);

        void setSize(int size); 

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string() const;
};

#endif 