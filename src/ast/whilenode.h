#ifndef WHILE_NODE_H
#define WHILE_NODE_H

#include <iostream>

#include "astnode.h"
#include "statementnode.h"
#include "exprnode.h"
#include "blocknode.h"
#include "../commons/defs.h"

class WhileNode : public StatementNode, public ASTNode {
    ExprNode *cond;
    BlockNode *block;

    public:
        WhileNode() {}
        
        WhileNode(ExprNode *cond, BlockNode *block) : cond(cond), block(block) {}

        ~WhileNode();

        ExprNode *getCond();

        BlockNode *getBlock();

        void setCond(ExprNode *cond);

        void setBlock(BlockNode *block);

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();
};

#endif 