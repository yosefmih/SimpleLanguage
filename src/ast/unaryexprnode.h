#ifndef UNARYEXPRNODE_H
#define UNARYEXPRNODE_H

#include "exprnode.h"

class UnaryExprNode : public ExprNode {
    Token *opToken;
    ExprNode *child;
    public:
        UnaryExprNode(Token *token, ExprNode *child);

        ~UnaryExprNode();

        void setOp(Token *token); 

        void setChild(ExprNode *child); 

        Token *getOp();

        ExprNode *getChild();

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();
};

#endif 