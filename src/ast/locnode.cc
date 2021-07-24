#include "locnode.h"

#include "../commons/defs.h"
#include "../commons/common.h"

LocNode::~LocNode() {
    delete identifier;

    if (index) {
        delete index;
    }
}

LitNode *LocNode::getIdentifier() {
    return identifier;
}

ExprNode *LocNode::getIndex() {
    return index; 
}

void LocNode::setIdentifier(LitNode *identifier) {
    this->identifier = identifier;
}

void LocNode::setIndex(ExprNode *index) {
    this->index = index; 
}

void LocNode::print(uint32_t depth, std::ostream& printTo) {
    printTo << indentationAtDepth(depth) << " ";
    identifier->print(depth, printTo);
    if (index) {
        printTo << "[" << string(*index) << "]";
    }
}

LocNode::operator string() const {
    string str = string(*identifier);
    if (index) {
        str += "[" + string(*index) + "]";
    }
    return str;
}

