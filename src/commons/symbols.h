#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "defs.h"

class Symbols {
    public:
        inline const static char EQUAL = '=',
                                 OP_GREATER = '>',
                                 OP_LESS = '<',
                                 OP_PLUS = '+',
                                 OP_MINUS = '-',
                                 OP_TIMES = '*',
                                 OP_DIVIDE = '/',
                                 OP_MOD = '%',
                                 AMPERSAND = '&',
                                 PIPE = '|',
                                 LPAREN = '(',
                                 RPAREN = ')',
                                 LBRACE = '{',
                                 RBRACE = '}',
                                 LBRACKET = '[',
                                 RBRACKET = ']',
                                 EXCALAMATION = '!',
                                 COMMA = ',',
                                 SEMICOLON = ';',
                                 SINGLE_QUOTES = '\'',
                                 DOUBLE_QUOTES = '\"';
        
        // compound operators 
        inline const static string PLUS_EQUAL = "+=",
                                   MINUS_EQUAL = "-=",
                                   INCREMENT = "++",
                                   DECREMENT = "--",
                                   OP_GEQ = ">=",
                                   OP_EQ = "==",
                                   OP_NEQ = "!=",
                                   OP_LEQ = "<=",
                                   OP_AND = "&&",
                                   OP_OR = "||";          
};

#endif 