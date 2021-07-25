#ifndef BREAK_CONTINUE_NODE_H
#define BREAK_CONTINUE_NODE_H

#include <iostream>

#include "statementnode.h"
#include "../commons/defs.h"
#include "../scanner/token.h"

class BreakContinueNode : public StatementNode {
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