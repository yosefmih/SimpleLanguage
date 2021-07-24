#ifndef WHILE_NODE_H
#define WHILE_NODE_H

#include <iostream>

#include "astnode.h"
#include "exprnode.h"
#include "blocknode.h"
#include "../commons/defs.h"

class WhileNode : public ASTNode {
    ExprNode *expr;
    BlockNode *block;

    public:
        WhileNode(ExprNode *expr, BlockNode *block) : expr(expr), block(block) {}

        ~WhileNode();

        ExprNode *getExpr();

        BlockNode *getBlock();

        void setExpr(ExprNode *expr);

        void setBlock(BlockNode *block);

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string() const;
};

#endif 