#include "ifelsenode.h"
#include "../commons/common.h"

IfElseNode::~IfElseNode() {
    delete cond;
    delete block;
    if (elifNode) delete elifNode;
    if (elseBlock) delete elseBlock;
}

ExprNode *IfElseNode::getCond() {
    return cond; 
}

BlockNode *IfElseNode::getBlock() {
    return block;
}

IfElseNode *IfElseNode::getElif() {
    return elifNode; 
}

BlockNode *IfElseNode::getElseBlock() {
    return elseBlock;
}

void IfElseNode::setCond(ExprNode *cond) {
    this->cond = cond; 
}

void IfElseNode::setBlock(BlockNode *block) {
    this->block = block; 
}

void IfElseNode::setElifNode(IfElseNode *elifNode) {
    this->elifNode = elifNode;
}

void IfElseNode::setElseBlock(BlockNode *block) {
    this->block = block;
}

void IfElseNode::print(uint32_t depth, std::ostream& printTo) {
    string indentation = indentationAtDepth(depth);
    printTo << indentation << "IfElse(" << std::endl;
    cond->print(depth+1, printTo);
    printTo << indentation << ")" << std::endl;

    block->print(depth+1, printTo);
    if (elifNode) elifNode->print(depth+1, printTo);

    if (elseBlock) {
        printTo << indentation << "else" << std::endl; 
        elseBlock->print(depth+1, printTo);
    }
}

IfElseNode::operator string() {
    string str = "If (" + string(*cond) + ")" + string(*block);
    if (elifNode) {
        str += string(*elifNode);
    }
    if (elseBlock) {
        str += string(*elseBlock);
    }
    return str;
} 