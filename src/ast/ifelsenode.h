#ifndef IF_ELSE_NODE_H
#define IF_ELSE_NODE_H

#include <iostream>

#include "astnode.h"
#include "statementnode.h"
#include "exprnode.h"
#include "blocknode.h"
#include "../commons/defs.h"

class IfElseNode : public StatementNode, public ASTNode {
    ExprNode *cond; 
    BlockNode *block;
    // if the if sequence has an elif, everything after the first if 
    // is contained in another else if statement
    IfElseNode *elifNode; 
    // set when there is an else. If there is an elif in the sequence, 
    // only the last elif will have an else
    BlockNode *elseBlock;

    public:
        IfElseNode() {};

        IfElseNode(ExprNode *cond, BlockNode *block, IfElseNode *elifNode) :
            cond(cond),
            block(block),
            elifNode(elifNode) {}

        IfElseNode(ExprNode *cond, BlockNode *block, BlockNode *elseBlock) : 
            cond(cond),
            block(block),
            elseBlock(elseBlock) {}

        ~IfElseNode();

        ExprNode *getCond();

        BlockNode *getBlock();

        IfElseNode *getElif();

        BlockNode *getElseBlock();

        void setCond(ExprNode *cond);

        void setBlock(BlockNode *block);

        void setElifNode(IfElseNode *elifNode);

        void setElseBlock(BlockNode *block);

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();
};

#endif 