#ifndef EXPRNODE_H
#define EXPRNODE_H
 
#include "astnode.h"
#include "../commons/defs.h"
#include "../scanner/token.h"

class ExprNode : public ASTNode {
    public:
        enum ExprNodeType {
            UNDEFINED, LITERAL, LOCATION, BINARY, UNARY, METHOD_CALL
        }; 

    protected:
        ExprNodeType type; 

    public:
        ExprNode(ExprNodeType type) : type(type){} 

        ExprNode() : type(UNDEFINED){}

        ExprNodeType getType();
};

#endif 