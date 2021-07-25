#ifndef LOCNODE_H
#define LOCNODE_H

#include "exprnode.h"
#include "literlanode.h"
#include "../scanner/token.h"
#include "../commons/defs.h"

class LocNode : public ExprNode {
    LitNode *identifier;
    // index is set in the case that the location is an array
    ExprNode *index; 
    public:
        LocNode(LitNode *identifier, ExprNode *index) : identifier(identifier), index(index) {}

        LocNode(LitNode *identifier) : identifier(identifier) {}

        ~LocNode();

        LitNode *getIdentifier();

        ExprNode *getIndex();

        void setIdentifier(LitNode *identifier);

        void setIndex(ExprNode *index);

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();

};

#endif