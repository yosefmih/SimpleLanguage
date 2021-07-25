#ifndef ASSIGN_NODE_H
#define ASSIGN_NODE_H

#include <iostream>

#include "astnode.h"
#include "locnode.h"
#include "exprnode.h"
#include "../scanner/token.h"
#include "../commons/defs.h"

class AssignNode : public ASTNode {
    LocNode *location;
    Token *assignOp;
    ExprNode *expr; 
    public:
        AssignNode(LocNode *location, Token *assignOp, ExprNode *expr);

        AssignNode(LocNode *location, Token *assignOp);

        ~AssignNode();

        LocNode *getLocation();

        Token *getAssignmentOp();

        ExprNode *getExpr(); 

        void setLocation(LocNode *location);

        void setAssignmentOp(Token *token);

        void setExpr(ExprNode *expr);

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string();
};

#endif 