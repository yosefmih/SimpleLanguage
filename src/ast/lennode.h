#ifndef LEN_NODE_H
#define LEN_NODE_H

#include <iostream>

#include "exprnode.h"
#include "literalnode.h"
#include "../commons/defs.h"

class LenNode : public ExprNode {
    LitNode *arrayVar;

    public:
        LenNode(LitNode *arrayVar) : arrayVar(arrayVar) {}

        LenNode() {}

        ~LenNode();

        LitNode *getArrayVar();

        void setArrayVar(LitNode *arrayVar);

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();
};

#endif 