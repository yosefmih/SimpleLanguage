#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include <iostream>
#include <vector>

#include "astnode.h"
#include "vardeclnode.h"
#include "../commons/defs.h"
#include "../symbol_table/symbol_table.h"
#include "../symbol_table/signature.h"

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

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();

        virtual void buildEnv(SymbolTable *env);
};

#endif 