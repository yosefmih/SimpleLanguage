#ifndef FOR_NODE_H
#define FOR_NODE_H

#include <iostream>

#include "astnode.h"
#include "exprnode.h"
#include "assignnode.h"
#include "blocknode.h"
#include "../commons/defs.h"
#include "../symbol_table/symbol_table.h"

class ForNode : public ASTNode {
    AssignNode *initiate;
    ExprNode *cond;
    AssignNode *update;
    BlockNode *block;

    public:
        ForNode() {}
        
        ForNode(AssignNode *initiate, ExprNode *cond, AssignNode *update, BlockNode *block) :
            initiate(initiate),
            cond(cond),
            update(update),
            block(block) {} 
        
        ~ForNode();

        AssignNode *getInitiate();

        ExprNode *getCond();

        AssignNode *getUpdate();

        BlockNode *getBlock();

        void setInitiate(AssignNode *initiate);

        void setCond(ExprNode *cond);

        void setUpdate(AssignNode *update);

        void setBlock(BlockNode *block);
        
        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();

        void buildEnv(SymbolTable *env);
};

#endif 