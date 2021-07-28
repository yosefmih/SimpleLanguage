#include "literalnode.h"

#include "../commons/defs.h"
#include "../commons/common.h"
#include "../scanner/token.h"


LitNode::~LitNode() {
    delete token; 
}

Token *LitNode::getToken() {
    return token; 
}

void LitNode::setToken(Token *token) {
    this->token = token; 
}

void LitNode::print(uint32_t depth, std::ostream& printTo) {
    printTo << indentationAtDepth(depth) << token->value; 
}

LitNode::operator string() {
    return "LIT " + token->toString();
}