#include "assignnode.h" 

#include <assert.h>

#include "../commons/defs.h"
#include "../commons/common.h"

AssignNode::AssignNode(LocNode *location, Token *assignOp, ExprNode *expr) {
    assert(assignOp->isAssignOp());
    this->location = location;
    this->assignOp = assignOp;
    this->expr = expr; 
}

AssignNode::AssignNode(LocNode *location, Token *assignOp) {
    assert(assignOp->isCompoundAssignOp()); 
    this->location = location;
    this->assignOp = assignOp;
}

AssignNode::~AssignNode() {
    delete location;
    delete assignOp;
    if (expr) {
        delete expr; 
    }
}

LocNode *AssignNode::getLocation() {
    return location; 
}

Token *AssignNode::getAssignmentOp() {
    return assignOp; 
}

ExprNode *AssignNode::getExpr() {
    return expr; 
}

void AssignNode::setLocation(LocNode *location) {
    this->location = location;
}

void AssignNode::setAssignmentOp(Token *token) {
    assignOp = token;
}

void AssignNode::setExpr(ExprNode *expr) {
    this->expr = expr;
}

void AssignNode::print(uint32_t depth, std::ostream& printTo) {
    printTo << indentationAtDepth(depth) << string(*location) << " " << assignOp->value; 
    if (expr) {
        printTo << " ";
        expr->print(depth, printTo);
    }
    printTo << std::endl;
}

AssignNode::operator string() const {
    string str = string(*location) + " " + assignOp->value + " ";
    if (!expr) {
        str  += string(*expr);
    }
    return str;
}