#include "symbol_table.h"

SymbolTable::~SymbolTable() {
    auto childIterator = children.begin();
    while (childIterator != children.end()) {
        if (*childIterator) {
            delete *childIterator;
        }
        childIterator++;
    }

    auto signIt = table.begin();
    while (signIt != table.end()) {
        if(signIt->second) {
            delete signIt->second;
        }
        signIt++;
    }
}

SymbolTable *SymbolTable::getParent() {
    return parent; 
}

std::vector<SymbolTable*>& SymbolTable::getChildren() {
    return children; 
}

std::map<string, Signature*> SymbolTable::getTable() {
    return table; 
}

void SymbolTable::setParent(SymbolTable *parent) {
    this->parent = parent; 
}

void SymbolTable::addChild(SymbolTable *child) {
    children.push_back(child);
}

void SymbolTable::addSymbol(string identifier, Signature *signature) {
    table.insert({identifier, signature});
}

void SymbolTable::addSymbol(Signature *signature) {
    string identifier = signature->getToken()->value; 
    addSymbol(identifier, signature);
}

bool SymbolTable::symbolExists(string symbol) {
    if (table.find(symbol) != table.end()) {
        return true;
    } else if (parent != nullptr) {
        return parent->symbolExists(symbol);
    }
    return false;
}

Signature *SymbolTable::getSignature(string symbol) {
    auto signIt = table.find(symbol);
    if (signIt != table.end()) {
        return signIt->second;
    } else if (parent) {
        return parent->getSignature(symbol);
    }
    return nullptr;
}