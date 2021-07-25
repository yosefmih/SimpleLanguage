#ifndef VARIABLE_DECL_NODE_H
#define VARIABLE_DECL_NODE_H

#include <iostream>

#include "astnode.h"
#include "literlanode.h"
#include "../commons/defs.h"
#include "../scanner/token.h"

class VarDeclNode : public ASTNode {
    Token *typeToken;
    LitNode *identifier;
    // this denotes the size when the variable is an array
    // it should be set to -1 if the variable is not an array 
    int size;     
    
    public:
        VarDeclNode(LitNode *identifier, Token *typeToken, int size) :
            identifier(identifier),
            typeToken(typeToken),
            size(size) {}

        VarDeclNode(LitNode *identifier, Token *typeToken) : 
            identifier(identifier),
            typeToken(typeToken),
            size(-1) {} 

        ~VarDeclNode();

        LitNode *getIdentifier();

        Token *getType();

        int getSize();

        bool isArray();

        void setIdentifier(LitNode *identifier);

        void setType(Token *typeToken);

        void setSize(int size); 

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();
};

#endif 