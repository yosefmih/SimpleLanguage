#ifndef BINARYEXPRNODE_H
#define BINARYEXPRNODE_H 

#include "exprnode.h"
#include "../commons/defs.h"
#include "../scanner/token.h"
#include "../symbol_table/symbol_table.h"

class BinaryExprNode : public ExprNode {
    Token *opToken;
    ExprNode *leftChild;
    ExprNode *rightChild;
    
    public:
        BinaryExprNode(Token *token, ExprNode *leftChild, ExprNode *rightChild); 

        ~BinaryExprNode();

        void setOp(Token *token);

        void setLeftChild(ExprNode *leftChild);
        
        void setRightChild(ExprNode *setRightChild);

        virtual Token *getOp(); 

        ExprNode *getLeftChild();

        ExprNode *getRightChild(); 

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();

        virtual void buildEnv(SymbolTable *env);
};

#endif 