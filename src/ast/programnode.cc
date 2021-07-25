#include "programnode.h"
#include "../commons/common.h"

ProgramNode::~ProgramNode() {
    deleteVectorMembers(imports);
    deleteVectorMembers(vars);
    deleteVectorMembers(methods);
}

std::vector<ImportNode*>& ProgramNode::getImports() {
    return imports;
}

std::vector<VarDeclNode*>& ProgramNode::getVars() {
    return vars;
}

std::vector<MethodDeclNode*>& ProgramNode::getMethods() {
    return methods;
}

void ProgramNode::addImport(ImportNode *import) {
    imports.push_back(import);
}

void ProgramNode::addVar(VarDeclNode *var) {
    vars.push_back(var);
}

void ProgramNode::addMethod(MethodDeclNode *method) {
    methods.push_back(method);
}

void ProgramNode::print(uint32_t depth, std::ostream& printTo) {
    string indentation = indentationAtDepth(depth);
    printTo << indentation << "Program" << std::endl;
    printVector(imports, depth+1, printTo);
    printVector(vars, depth+1, printTo);
    printVector(methods, depth+1, printTo);
}

ProgramNode::operator string() {
    string str = "Program\n";
    str += toString(imports);
    str += toString(vars);
    str += toString(methods);
    return str;
}