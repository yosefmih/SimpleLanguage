#include "lennode.h"

#include "../commons/common.h"

LenNode::~LenNode() {
    if (arrayVar) {
        delete arrayVar;
    }
}

LitNode *LenNode::getArrayVar() {
    return arrayVar;
}

void LenNode::setArrayVar(LitNode *arrayVar) {
    this->arrayVar = arrayVar;
}

void LenNode::print(uint32_t depth, std::ostream& printTo) {
    string indentation = indentationAtDepth(depth);
    printTo << indentation << string(*this);
}

LenNode::operator string() {
    return "Len(" + string(*arrayVar)  + ")";
}