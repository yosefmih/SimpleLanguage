#ifndef METHOD_DECL_NODE_H
#define METHOD_DECL_NODE_H

#include <iostream>
#include <vector>

#include "astnode.h"
#include "literalnode.h"
#include "blocknode.h"
#include "methodargnode.h"
#include "../commons/defs.h"
#include "../symbol_table/symbol_table.h"
#include "../symbol_table/signature.h"
#include "../symbol_table/methods.h"

class MethodDeclNode : ASTNode {
    Token *returnType;
    LitNode *identifier;
    std::vector<MethodArgNode*> args;
    BlockNode *block;
    SymbolTable *methodEnv;
    MethodSignature *signature;

    public:
        MethodDeclNode() {};

        MethodDeclNode(Token *returnType, LitNode *identifier) : returnType(returnType), identifier(identifier) {}

        ~MethodDeclNode();

        Token *getReturnType();

        LitNode *getIdentifier();

        std::vector<MethodArgNode*>& getArgs(); 

        BlockNode *getBlock();

        void setReturnType(Token *returnType);

        void setIdentifier(LitNode *identifier);

        void addArg(MethodArgNode *arg);

        void setBlock(BlockNode *block);

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();

        void buildSignature();

        MethodSignature *getSignature();

        virtual void buildEnv(SymbolTable *env);
};

#endif 