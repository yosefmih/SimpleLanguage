#include "fornode.h"

#include "../commons/common.h"

ForNode::~ForNode() {
    delete initiate;
    delete cond;
    delete update;
    delete block;
}

AssignNode *ForNode::getInitiate() {
    return initiate;
}

ExprNode *ForNode::getCond() {
    return cond; 
}

AssignNode *ForNode::getUpdate() {
    return update; 
}

BlockNode *ForNode::getBlock() {
    return block;
}

void ForNode::setInitiate(AssignNode *initiate) {
    this->initiate = initiate; 
}

void ForNode::setCond(ExprNode *cond) {
    this->cond = cond;
}

void ForNode::setUpdate(AssignNode *update) {
    this->update = update; 
}

void ForNode::setBlock(BlockNode *block) {
    this->block = block; 
}

void ForNode::print(uint32_t depth, std::ostream& printTo) {
    string indentation = indentationAtDepth(depth);
    printTo << indentation << "For(" << std::endl;
    initiate->print(depth+1, printTo);
    printTo << indentation << ";" << std::endl; 
    cond->print(depth+1, printTo);
    printTo << indentation << ";" << std::endl; 
    update->print(depth+1, printTo);
    printTo << ")" << std::endl;
    block->print(depth+1, printTo);
}

ForNode::operator string() const {
    return "For(" + string(*initiate) + ";" + string(*cond) + ";" + string(*update) + ")" + string(*block);
}
