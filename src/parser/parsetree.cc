#include "parsetree.h"

#include <vector>
#include <string>

#include "../commons/defs.h"

NonTerminal::~NonTerminal() {
    for (auto ptree : children) {
        delete ptree;
    }
}

void NonTerminal::addChild(ParseTree *child) {
    children.push_back(child);
}

void NonTerminal::addChild(Token *child) {
    Terminal *childTerm = new Terminal(child);
    addChild(childTerm);
}

void NonTerminal::modifyLabel(string newLabel) {
    label = newLabel;
}