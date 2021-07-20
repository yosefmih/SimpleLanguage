#ifndef PARSETREE_H
#define PARSETREE_H

#include <vector>
#include <string> 

#include "../commons/defs.h"
#include "../scanner/token.h"

class ParseTree {
    public:
        ParseTree(){};
};

class NonTerminal : public ParseTree {
    public:
        // This is the label of this non terminal node 
        string label;

        // a list of the children of this node 
        std::vector<ParseTree*> children;
        
        NonTerminal(string label) : label(label){}; 

        NonTerminal(){};

        ~NonTerminal();

        /**
         * Adds a child to this NonTerminal node 
         *
         * @param child: a ptr to a ParseTree
         */
        void addChild(ParseTree *child);

        /**
         * Creates a Terminal node from this token and adds 
         * it as a child
         */
        void addChild(Token *child);

        /**
         * Changes the label of this node to newLabel
         * 
         * @param newLabel: a string
         */
        void modifyLabel(string newLabel);
};

class Terminal : public ParseTree {
    public:
        // The token this Terminal node represents
        Token *token;
        
        Terminal(Token *token) : token(token){};

        ~Terminal() {
            delete token;
        };
};

#endif 