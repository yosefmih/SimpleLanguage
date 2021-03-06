#ifndef IMPORT_NODE_H
#define IMPORT_NODE_H

#include <iostream>

#include "astnode.h"
#include "literalnode.h"
#include "../commons/defs.h"
#include "../symbol_table/symbol_table.h"

class ImportNode : public ASTNode {
    LitNode *identifier;


    public:
        ImportNode() {}
        
        ImportNode(LitNode *identifier) : identifier(identifier) {}
        
        ~ImportNode();

        LitNode *getIdentifier();

        void setIdentifier(LitNode *identifier);

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string(); 

        virtual void buildEnv(SymbolTable *env);

        void buildSignature();

        Signature *getSignature();   
};

#endif 