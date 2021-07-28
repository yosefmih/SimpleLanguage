#ifndef RETURN_NODE_H
#define RETURN_NODE_H

#include <iostream>

#include "astnode.h"
#include "statementnode.h"
#include "exprnode.h"
#include "../commons/defs.h"

class ReturnNode : public StatementNode, public ASTNode {
    ExprNode *expr; 

    public:
        ReturnNode() {};
        
        ReturnNode(ExprNode *expr) : expr(expr){}

        ~ReturnNode();

        ExprNode *getExpr();

        void setExpr(ExprNode *expr);

        virtual void print(uint32_t depth, std::ostream& printTo); 

        virtual operator string();
};

#endif 