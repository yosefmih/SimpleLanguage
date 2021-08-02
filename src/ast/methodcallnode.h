#ifndef METHOD_CALL_NODE_H
#define METHOD_CALL_NODE_H

#include <vector>
#include <iostream>

#include "exprnode.h"
#include "literalnode.h"
#include "../scanner/token.h"
#include "../commons/defs.h"

class MethodCallNode : public ExprNode {
    LitNode *identifier;
    std::vector<ExprNode*> args;
    
    public:
        MethodCallNode() {}

        MethodCallNode(LitNode *identifier) : identifier(identifier) {}
        
        MethodCallNode(const ExprNode &expr) {}

        ~MethodCallNode();

        LitNode *getIdentifier();

        std::vector<ExprNode*>& getArgs();

        void setIdentifier(LitNode *identifier);

        void addArg(ExprNode *arg);

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string(); 
};

#endif 