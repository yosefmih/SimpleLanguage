#include "binaryexprnode.h"

#include <assert.h>

#include "exprnode.h"
#include "../scanner/token.h"
#include "../commons/common.h"

BinaryExprNode::BinaryExprNode(Token *token, ExprNode *leftChild, ExprNode *rightChild) {
    assert(token->isBinaryOp());
    this->opToken = token;
    this->leftChild = leftChild;
    this->rightChild = rightChild;
    this->type = BINARY;
}


BinaryExprNode::~BinaryExprNode() {
    delete opToken; 
    delete leftChild;
    if (rightChild != nullptr) {
        delete rightChild; 
    }
}

void BinaryExprNode::setOp(Token *token){
    assert (token->isBinaryOp() || token->isUnaryOp());
    opToken = token; 
}

void BinaryExprNode::setLeftChild(ExprNode *leftChild) {
    this->leftChild = leftChild; 
}

void BinaryExprNode::setRightChild(ExprNode *rightChild) {
    this->rightChild = rightChild;
}

Token *BinaryExprNode::getOp() {
    return opToken;
}

ExprNode *BinaryExprNode::getLeftChild() {
    return leftChild; 
}

ExprNode *BinaryExprNode::getRightChild() {
    return rightChild;
}

void BinaryExprNode::print(uint32_t depth, std::ostream& printTo) {
    printTo << indentationAtDepth(depth) << opToken->value << std::endl;
    leftChild->print(depth+1, printTo);
    rightChild->print(depth+1, printTo);
}

BinaryExprNode::operator string() const {
    string str;
    str += opToken->value + " " + string(*leftChild) + "\n " + string(*rightChild); 
    return str; 
}







