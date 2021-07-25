#include "importnode.h"
#include "../commons/common.h"

ImportNode::~ImportNode() {
    delete identifier;
}

LitNode *ImportNode::getIdentifier() {
    return identifier;
}

void ImportNode::setIdentifier(LitNode *identifier) {
    this->identifier = identifier;
}

void ImportNode::print(uint32_t depth, std::ostream& printTo) {
    printTo << indentationAtDepth(depth) << "Import " << string(*identifier) << std::endl;
}

ImportNode::operator string() {
    return "Import " + string(*identifier);
}

