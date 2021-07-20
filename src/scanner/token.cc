#include "token.h"

#include <string>
#include <iostream>
#include <assert.h>

#include "../commons/defs.h"
#include "../commons/exceptions.h"
#include "../commons/keywords.h"
#include "../commons/symbols.h"

Token::Token(TokenType type, string value, uint32_t row, uint32_t col) {
    this->type = type;
    this->value = value;
    this->row = row;
    this->col = col;
}

Token::Token(uint32_t row, uint32_t col) {
    type = UNSPECIFIED;
    value = "";
    this->row = row;
    this->col = col;
}

bool Token::checkType(TokenType type) {
    return this->type == type;
}

bool Token::checkValue(string value) {
    return this->value == value;
}

bool Token::checkValue(char value) {
    // check if a single char value would be equal to the saved value
    // useful to check for symboles like parentheses 
    string valStr(1, value);
    return checkValue(valStr);
}

bool Token::isDataType() {
    return checkValue(Keywords::BOOL) || checkValue(Keywords::INT) || checkValue(Keywords::CHAR);
}

bool Token::isLiteral() {
    return checkType(INT_LITERAL) || checkType(BOOLEAN_LITERAL) || checkType(CHAR_LITERAL);
}

bool Token::isAssignOp() {
    return checkValue(Symbols::EQUAL) || checkValue(Symbols::PLUS_EQUAL) || checkValue(Symbols::MINUS_EQUAL); 
}

bool Token::isIncrementOp() {
    return checkValue(Symbols::INCREMENT) || checkValue(Symbols::DECREMENT);
}

bool Token::isBinaryOp() {
    return checkValue(Symbols::OP_AND) || 
           checkValue(Symbols::OP_OR) || 
           checkValue(Symbols::OP_MINUS) || 
           checkValue(Symbols::OP_PLUS) || 
           checkValue(Symbols::OP_TIMES) || 
           checkValue(Symbols::OP_DIVIDE) || 
           checkValue(Symbols::OP_EQ) ||
           checkValue(Symbols::OP_NEQ) ||
           checkValue(Symbols::OP_GREATER) || 
           checkValue(Symbols::OP_LESS) || 
           checkValue(Symbols::OP_LEQ) || 
           checkValue(Symbols::OP_GEQ) || 
           checkValue(Symbols::OP_MOD);
}

bool Token::isMorePrecedent(Token *anotherToken) {
    assert (!(isBinaryOp() && anotherToken->isAssignOp()));
    
    if (checkValue(Symbols::OP_TIMES) || checkValue(Symbols::OP_DIVIDE) || checkValue(Symbols::OP_MOD)) {
        return true;
    } else if (anotherToken->checkValue(Symbols::OP_TIMES) || anotherToken->checkValue(Symbols::OP_DIVIDE)  || anotherToken->checkValue(Symbols::OP_MOD)) {
        return false; 
    } else if (checkValue(Symbols::OP_MINUS) || checkValue(Symbols::OP_PLUS)) {
        return true;
    } else if (anotherToken->checkValue(Symbols::OP_MINUS) || anotherToken->checkValue(Symbols::OP_PLUS)) {
        return false;
    }
    return true;
}

bool Token::isCompoundAssignOp() {
    // assert (type == UNSPECIFIED);
    return checkValue(Symbols::PLUS_EQUAL) || checkValue(Symbols::MINUS_EQUAL);
}

Token::operator string() const {
    return getTokenTypeStr() + " '" + value + "' at " + std::to_string(row) + "," + std::to_string(col);
}

string Token::toString() const {
    string typeStr = type == TokenType::UNSPECIFIED ? " " 
                                                          : " " + getTokenTypeStr() + " "; 
    return std::to_string(row) + typeStr + value; 
}

string Token::getTokenTypeStr() const {
    switch (type) {
        case FILE_END:
            return "EOF";
        case UNSPECIFIED:
            return "UNSPECIFIED";
        case CHAR_LITERAL:
            return "CHARLITERAL";
        case INT_LITERAL:
            return "INTLITERAL";
        case BOOLEAN_LITERAL:
            return "BOOLEANLITERAL";
            break;
        case STRING_LITERAL:
            return "STRINGLITERAL";
        case IDENTIFIER:
            return "IDENTIFIER";
        default:
            throw Exception("Shouldn't be here");
    }
}