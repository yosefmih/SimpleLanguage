#include "vardeclnode.h"
#include "../commons/common.h"

VarDeclNode::~VarDeclNode() {
    delete identifier;
    delete typeToken;
}

LitNode *VarDeclNode::getIdentifier() {
    return identifier; 
}

Token *VarDeclNode::getType() {
    return typeToken;
}

int VarDeclNode::getSize() {
    return size;
}

bool VarDeclNode::isArray() {
    return size >= 0; 
}

void VarDeclNode::setIdentifier(LitNode *identifier) {
    this->identifier = identifier;
}

void VarDeclNode::setType(Token *typeToken) {
    this->typeToken = typeToken;
}

void VarDeclNode::setSize(int size) {
    this->size = size;
}

void VarDeclNode::print(uint32_t depth, std::ostream& printTo) {
    printTo << indentationAtDepth(depth) << typeToken->value << " ";
    identifier->print(depth, printTo);
    if (isArray()) {
        printTo << "[" << size << "]";
    }
    printTo << std::endl;
}

VarDeclNode::operator string() {
    return typeToken->value + " " + string(*identifier) +
            (size > 0 ? "[" + std::to_string(size) + "]" 
                      : "");
}