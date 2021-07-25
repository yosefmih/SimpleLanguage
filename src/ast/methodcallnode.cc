#include "methodcallnode.h"

#include "../commons/common.h"

MethodCallNode::~MethodCallNode() {
    delete identifier;
    deleteVectorMembers(args);
}

LitNode *MethodCallNode::getIdentifier() {
    return identifier;
}

std::vector<ExprNode*>& MethodCallNode::getArgs() {
    return args;
}

void MethodCallNode::setIdentifier(LitNode *identifier) {
    this->identifier = identifier; 
}

void MethodCallNode::addArg(ExprNode *arg) {
    args.push_back(arg);
}

void MethodCallNode::print(uint32_t depth, std::ostream& printTo) {
    printTo << indentationAtDepth(depth) << string(*identifier) << "(";
    ExprNode::printVector(args, depth+1, printTo);
    printTo <<  indentationAtDepth(depth) + ")";
}

MethodCallNode::operator string() {
    string str = string(*identifier) + "(\n";
    str += ExprNode::toString(args);
    str += ")";
    return str;
}