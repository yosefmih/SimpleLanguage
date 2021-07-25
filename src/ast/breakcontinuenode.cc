#include "breakcontinuenode.h"
#include "../commons/keywords.h"
#include "../commons/common.h"

BreakContinueNode::~BreakContinueNode() {
    delete token;
}

Token *BreakContinueNode::getToken() {
    return token;
}

void BreakContinueNode::setToken(Token *token) {
    this->token = token;
}

bool BreakContinueNode::isBreak() {
    return token->checkValue(Keywords::BREAK);
}

void BreakContinueNode::print(uint32_t depth, std::ostream& printTo) {
    printTo << indentationAtDepth(depth) << token->value << std::endl;
}

BreakContinueNode::operator string() {
    return token->value;
}


