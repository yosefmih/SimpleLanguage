#ifndef PROGRAM_NODE_H
#define PROGRAM_NODE_H

#include <iostream>
#include <vector>

#include "astnode.h"
#include "importnode.h"
#include "vardeclnode.h"
#include "methoddeclnode.h"
#include "../commons/defs.h"
#include "../symbol_table/symbol_table.h"

class ProgramNode : public ASTNode {
    std::vector<ImportNode*> imports;
    std::vector<VarDeclNode*> vars;
    std::vector<MethodDeclNode*> methods;
    SymbolTable *globalEnv;

    public:
        ~ProgramNode();

        std::vector<ImportNode*>& getImports();

        std::vector<VarDeclNode*>& getVars();

        std::vector<MethodDeclNode*>& getMethods();

        void addImport(ImportNode *import);

        void addVar(VarDeclNode *var);

        void addMethod(MethodDeclNode *method);

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();

        virtual void buildEnv(SymbolTable *env);    
};

#endif