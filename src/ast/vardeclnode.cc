#include "vardeclnode.h"

VarDeclNode::~VarDeclNode() {
    delete identifier;
}

LitNode *VarDeclNode::getIdentifier() {
    return identifier; 
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

void VarDeclNode::setSize(int size) {
    this->size = size;
}

void VarDeclNode::print(uint32_t depth, std::ostream& printTo) {
    identifier->print(depth, printTo);
    if (isArray()) {
        printTo << "[" << size << "]";
    }
    printTo << std::endl;
}

VarDeclNode::operator string() const {
    return string(*identifier) + (size > 0 ? "[" + std::to_string(size) + "]" 
                                            : "");
}