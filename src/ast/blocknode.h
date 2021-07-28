#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include <iostream>
#include <vector>

#include "astnode.h"
#include "vardeclnode.h"
#include "../commons/defs.h"

class BlockNode : public ASTNode {
    std::vector<VarDeclNode*> vars;
    std::vector<ASTNode*> statements;
    public:
        BlockNode(){};

        ~BlockNode();

        std::vector<VarDeclNode*>& getVarDecls();

        std::vector<ASTNode*>& getStatements();

        void addVarDecl(VarDeclNode *var);

        void addStatement(ASTNode *astNode);
        // void addStatement(ForNode *forNode);

        // void addStatement(BreakContinueNode *BreakContinueNode);

        // void addStatement(WhileNode *whileNode);

        // void addStatement(MethodCallNode *MethodCallNode);

        // void addStatement(ReturnNode *returnNode);

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();
};

#endif 