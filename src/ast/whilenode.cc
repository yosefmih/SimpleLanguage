#include "whilenode.h"

#include "../commons/common.h"

WhileNode::~WhileNode() {
    delete cond;
    delete block;
}

ExprNode *WhileNode::getCond() {
    return cond;
}

BlockNode *WhileNode::getBlock() {
    return block; 
}

void WhileNode::setCond(ExprNode *cond) {
    this->cond = cond; 
}

void WhileNode::setBlock(BlockNode *block) {
    this->block = block;
}

void WhileNode::print(uint32_t depth, std::ostream& printTo) {
    string indentaion = indentationAtDepth(depth);
    printTo << indentaion << "While(" << std::endl;
    cond->print(depth+1, printTo);
    printTo << indentaion << ")" << std::endl;
    block->print(depth+1, printTo); 
}

WhileNode::operator string() {
    return "While(" + string(*cond) + ")" + string(*block);
}