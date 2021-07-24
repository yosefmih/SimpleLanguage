#include "unaryexprnode.h"

#include "assert.h"

#include "../commons/defs.h"
#include "../commons/common.h"

UnaryExprNode::UnaryExprNode(Token *token, ExprNode *child) {
    assert(token->isUnaryOp()); 
    this->opToken = token;
    this->child = child;
    type = UNARY;
}

UnaryExprNode::~UnaryExprNode() {
    delete this->opToken; 
    delete this->child;
}

void UnaryExprNode::setOp(Token *token) {
    opToken = token; 
}

void UnaryExprNode::setChild(ExprNode *child) {
    this->child = child; 
}

Token *UnaryExprNode::getOp() {
    return opToken; 
}

ExprNode *UnaryExprNode::getChild() {
    return child;
}

void UnaryExprNode::print(uint32_t depth, std::ostream& printTo) {
    printTo << indentationAtDepth(depth) << opToken->value << "(";
    child->print(depth+1, printTo);
    printTo << ")" << std::endl;  
}

UnaryExprNode::operator string() const {
    string str;
    str += opToken->value + " " + string(*child); 
    return str; 
}