#include "token.h"

#include <string>
#include <iostream>

#include "../commons/defs.h"
#include "../commons/exceptions.h"

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