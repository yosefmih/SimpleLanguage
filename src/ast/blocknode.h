#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include <iostream>
#include <vector>

#include "astnode.h"
#include "statementnode.h"
#include "vardeclnode.h"
#include "../commons/defs.h"

class BlockNode : public ASTNode {
    std::vector<VarDeclNode*> vars;
    std::vector<StatementNode*> statements;

    public:
        BlockNode(){};

        ~BlockNode();

        std::vector<VarDeclNode*>& getVarDecls();

        std::vector<StatementNode*>& getStatements();

        void addVarDecl(VarDeclNode *var);

        void addStatement(StatementNode *statement);

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();
};

#endif 