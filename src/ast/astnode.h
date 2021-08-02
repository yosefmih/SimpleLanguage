#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>
#include <vector>

#include "../commons/defs.h"
#include "../scanner/token.h"
#include "../symbol_table/symbol_table.h"

class ASTNode {
    protected:
        SymbolTable *immediateEnv;

    public:
        virtual void print(uint32_t depth, std::ostream& printTo) = 0;

        virtual operator string() = 0;

        virtual bool checkSemantic();

        virtual void buildEnv(SymbolTable *env);

        SymbolTable *getEnv();

        // using  VecIterator = std::vector<ASTNode*>::iterator;
        template <typename T>
        void printVector(T begin, T end, uint32_t depth, std::ostream& printTo);

        template <typename T>
        void deleteVectorMembers(T begin, T end);

        template <typename T>
        string toString(T begin, T end);

        void addVar(ASTNode *var) {};
};

#endif 