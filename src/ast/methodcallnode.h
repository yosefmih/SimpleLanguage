#ifndef METHOD_CALL_NODE_H
#define METHOD_CALL_NODE_H

#include <vector>

#include "exprnode.h"
#include "literlanode.h"
#include "statementnode.h"
#include "../scanner/token.h"
#include "../commons/defs.h"

class MethodCallNode : public ExprNode, public StatementNode {
    LitNode *identifier;
    std::vector<ExprNode*> args;
    public:
        MethodCallNode(LitNode *identifier) : identifier(identifier){}

        ~MethodCallNode();

        LitNode *getIdentifier();

        std::vector<ExprNode*>& getArgs();

        void setIdentifier(LitNode *identifier);

        void addArg(ExprNode *arg);

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string(); 
};

#endif 