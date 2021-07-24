#include "methodcallnode.h"

#include "../commons/common.h"

MethodCallNode::~MethodCallNode() {
    delete identifier;

    auto argIterator = args.begin();
    while (argIterator != args.end()) {
        delete *argIterator;
        argIterator++;
    }
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
    
    auto argIterator = args.begin();
    while (argIterator != args.end()) {
        (*argIterator)->print(depth+1, printTo);
        printTo << std::endl; 
    }

    printTo << ")";
}