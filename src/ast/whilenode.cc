#include "whilenode.h"

#include "../commons/common.h"

WhileNode::~WhileNode() {
    delete expr;
    delete block;
}

ExprNode *WhileNode::getExpr() {
    return expr;
}

BlockNode *WhileNode::getBlock() {
    return block; 
}

void WhileNode::setExpr(ExprNode *expr) {
    this->expr = expr; 
}

void WhileNode::setBlock(BlockNode *block) {
    this->block = block;
}

void WhileNode::print(uint32_t depth, std::ostream& printTo) {
    string indentaion = indentationAtDepth(depth);
    printTo << indentaion << "While(" << std::endl;
    expr->print(depth+1, printTo);
    printTo << indentaion << ")" << std::endl;
    block->print(depth+1, printTo); 
}

WhileNode::operator string() const {
    return "While(" + string(*expr) + ")" + string(*block);
}