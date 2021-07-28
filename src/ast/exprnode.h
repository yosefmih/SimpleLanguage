#ifndef EXPRNODE_H
#define EXPRNODE_H
 
#include <iostream>

#include "astnode.h"
#include "../commons/defs.h"
#include "../scanner/token.h"
#include "../commons/exceptions.h"

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

        virtual void print(uint32_t depth, std::ostream& printTo) { throw Exception("Undefined method"); }

        virtual operator string() { throw Exception("Undefined method"); }
};

#endif 