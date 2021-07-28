#ifndef BREAK_CONTINUE_NODE_H
#define BREAK_CONTINUE_NODE_H

#include <iostream>

#include "astnode.h"
#include "../commons/defs.h"
#include "../scanner/token.h"

class BreakContinueNode : public ASTNode {
    Token *token;

    public:
        BreakContinueNode(Token *token) : token(token) {} 

        ~BreakContinueNode();

        Token *getToken();

        void setToken(Token *token);

        bool isBreak();

        virtual void print(uint32_t depth, std::ostream& printTo);

        virtual operator string(); 
};

#endif 