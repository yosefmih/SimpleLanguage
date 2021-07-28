#include "blocknode.h"
#include "../commons/common.h"

BlockNode::~BlockNode() {
    // deleteVectorMembers(vars.begin(), vars.end());
    auto varsIterator = vars.begin();
    while (varsIterator != vars.end()) {
        delete *varsIterator;
        varsIterator++;
    }
    
    auto statIterator = statements.begin();
    while (statIterator != statements.end()) {
        delete *statIterator;
        statIterator++;
    }
    // deleteVectorMembers(statements.begin(), statements.end());
}

std::vector<VarDeclNode*>& BlockNode::getVarDecls() {
    return vars;
}

std::vector<ASTNode*>& BlockNode::getStatements() {
    return statements;
}

void BlockNode::addVarDecl(VarDeclNode *var) {
    this->vars.push_back(var);
}

void BlockNode::addStatement(ASTNode *astNode) {
    this->statements.push_back(astNode);
}

void BlockNode::print(uint32_t depth, std::ostream& printTo) {
    string indentation = indentationAtDepth(depth);
    printTo << indentation << "{" << std::endl;

    auto varIterator = vars.begin();
    while (varIterator != vars.end()) {
        (*varIterator)->print(depth+1, printTo);
        varIterator++;
    }

    // printVector(vars.begin(), vars.end(), depth+1, printTo);

    auto statementIterator = statements.begin();
    while (statementIterator != statements.end()) {
        ((ASTNode*)*statementIterator)->print(depth+1, printTo);
        statementIterator++;
    } 
    // printVector(statements.begin(), statements.end(), depth+1, printTo);

    printTo << indentation << "}" << std::endl;
}

BlockNode::operator string() {
    string str = "{\n";
    // str += toString(vars.begin(), vars.end());

    auto varIterator = vars.begin();
    while (varIterator != vars.end()) {
        str += string(**varIterator);
        varIterator++;
    }

    auto statementIterator = statements.begin();
    while (statementIterator != statements.end()) {
        str += string(*(ASTNode*)*statementIterator);
        statementIterator++;
    } 

    str += "}";
    return str;
}