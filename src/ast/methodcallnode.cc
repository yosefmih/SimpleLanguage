#include "methodcallnode.h"

#include "../commons/common.h"

MethodCallNode::~MethodCallNode() {
    delete identifier;
    // deleteVectorMembers(args.begin(), args.end());
    auto argsIterator = args.begin();
    while (argsIterator != args.end()) {
        delete *argsIterator;
        argsIterator++;
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
    // args.push_back(arg);
    std::cout << args.size() << std::endl;
    args.push_back(new ExprNode());
}

void MethodCallNode::print(uint32_t depth, std::ostream& printTo) {
    printTo << indentationAtDepth(depth) << string(*identifier) << "(";
    // printVector(args.begin(), args.end(), depth+1, printTo);
    printTo <<  indentationAtDepth(depth) + ")";
}

MethodCallNode::operator string() {
    string str = string(*identifier) + "(\n";
    // str += toString(args.begin(), args.end());
    str += ")";
    return str;
}