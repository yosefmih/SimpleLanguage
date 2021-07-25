#include "blocknode.h"
#include "../commons/common.h"

BlockNode::~BlockNode() {
    deleteVectorMembers(vars);
    deleteVectorMembers(statements);
}

std::vector<VarDeclNode*>& BlockNode::getVarDecls() {
    return vars;
}

std::vector<StatementNode*>& BlockNode::getStatements() {
    return statements;
}

void BlockNode::addVarDecl(VarDeclNode *var) {
    this->vars.push_back(var);
}

void BlockNode::addStatement(StatementNode *statement) {
    this->statements.push_back(statement);
}

void BlockNode::print(uint32_t depth, std::ostream& printTo) {
    string indentation = indentationAtDepth(depth);
    printTo << indentation << "{" << std::endl;

    auto varIterator = vars.begin();
    while (varIterator != vars.end()) {
        (*varIterator)->print(depth+1, printTo);
        varIterator++;
    }

    auto statementIterator = statements.begin();
    while (statementIterator != statements.end()) {
        (*statementIterator)->print(depth+1, printTo);
        statementIterator++;
    } 

    printTo << indentation << "}" << std::endl;
}

BlockNode::operator string() {
    string str = "{\n";
    str += toString(vars);

    auto varIterator = vars.begin();
    while (varIterator != vars.end()) {
        str += string(**varIterator);
        varIterator++;
    }

    auto statementIterator = statements.begin();
    while (statementIterator != statements.end()) {
        str += string(**statementIterator);
        statementIterator++;
    } 

    str += "}";
    return str;
}