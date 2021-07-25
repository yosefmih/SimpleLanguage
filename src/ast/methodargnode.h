#ifndef METHOD_ARG_NODE_H
#define METHOD_ARG_NODE_H

#include "vardeclnode.h"
#include "literlanode.h"
#include "../scanner/token.h"

class MethodArgNode : public VarDeclNode {
    public:
        MethodArgNode(LitNode *identifier, Token *typeToken) : VarDeclNode(identifier, typeToken) {}
};

#endif 