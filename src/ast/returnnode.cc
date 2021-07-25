#include "returnnode.h"

#include "../commons/common.h"

ReturnNode::~ReturnNode() {
    if (expr) {
        delete expr;
    }
}

ExprNode *ReturnNode::getExpr() {
    return expr; 
}

void ReturnNode::setExpr(ExprNode *expr) {
    this->expr = expr; 
}

void ReturnNode::print(uint32_t depth, std::ostream& printTo) {
    string indentation = indentationAtDepth(depth);
    printTo << indentation << "Return" << std::endl;
    if (expr) {
        expr->print(depth+1, printTo);
    }
}

ReturnNode::operator string() {
    return "return" + (expr == nullptr)? "" : string(*expr);
}

