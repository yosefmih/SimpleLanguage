#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include <map>

#include "signature.h"
#include "../commons/common.h"

class SymbolTable {
    // one parent, multiple children
    SymbolTable *parent;
    std::vector<SymbolTable*> children;
    std::map<string, Signature*> table;

    public:
        SymbolTable() {};

        ~SymbolTable();

        SymbolTable *getParent();

        std::vector<SymbolTable*>& getChildren();

        std::map<string, Signature*> getTable();

        void setParent(SymbolTable *parent);

        void addChild(SymbolTable *child);

        void addSymbol(string identifier, Signature *signature);

        // in this case, the symbol identifier is extracted from signature itself
        void addSymbol(Signature *signature);
};

#endif 