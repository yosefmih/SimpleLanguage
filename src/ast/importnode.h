#ifndef IMPORT_NODE_H
#define IMPORT_NODE_H

#include <iostream>

#include "astnode.h"
#include "literlanode.h"
#include "../commons/defs.h"

class ImportNode : public ASTNode {
    LitNode *identifier;

    public:
        ImportNode(LitNode *identifier) : identifier(identifier) {}
        
        ~ImportNode();

        LitNode *getIdentifier();

        void setIdentifier(LitNode *identifier);

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();    
};

#endif 