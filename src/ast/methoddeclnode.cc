#include "methoddeclnode.h"
#include "../commons/common.h"

MethodDeclNode::~MethodDeclNode() {
    delete returnType;
    delete identifier;
    auto argsIterator = args.begin();
    while (argsIterator != args.end()) {
        delete *argsIterator;
        argsIterator++;
    }}

Token *MethodDeclNode::getReturnType() {
    return returnType;
}

LitNode *MethodDeclNode::getIdentifier() {
    return identifier;
}

std::vector<MethodArgNode*>& MethodDeclNode::getArgs() {
    return args; 
}

BlockNode *MethodDeclNode::getBlock() {
    return block;
}

void MethodDeclNode::setReturnType(Token *returnType) {
    this->returnType = returnType;
}

void MethodDeclNode::setIdentifier(LitNode *identifier) {
    this->identifier = identifier; 
}

void MethodDeclNode::addArg(MethodArgNode *arg) {
    this->args.push_back(arg);
}

void MethodDeclNode::setBlock(BlockNode *block) {
    this->block = block; 
}

void MethodDeclNode::print(uint32_t depth, std::ostream& printTo) {
    string indentation = indentationAtDepth(depth);
    printTo << indentation << returnType->value << " ";
    identifier->print(depth, printTo);
    printTo << "(";
    auto argIterator = args.begin();
    while (argIterator != args.end()) {
        printTo << ",";
        (*argIterator)->print(depth, printTo);
        argIterator++;
    }
    printTo << ")" << std::endl;
    block->print(depth+1, printTo);
}

MethodDeclNode::operator string() {
    string str = returnType->value + " " + string(*identifier) + "(";
    auto argIterator = args.begin();
    while (argIterator != args.end()) {
        str += "," + string(**argIterator);
        argIterator++; 
    }
    str += ")" + string(*block);
    return str;
}