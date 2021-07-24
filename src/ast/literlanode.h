#ifndef LITNODE_H
#define LITNODE_H

#include "astnode.h"
#include "exprnode.h"
#include "../commons/defs.h"
#include "../scanner/token.h"

class LitNode : public ExprNode {
    Token *token;
    public:
        LitNode(Token *token) : token(token) {}

        ~LitNode();

        Token *getToken();

        void setToken(Token *token);

        virtual void print(uint32_t depth, std::ostream& printTo); 

        virtual operator string() const;
};

#endif 