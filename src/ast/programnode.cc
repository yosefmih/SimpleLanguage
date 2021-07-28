#include "programnode.h"
#include "../commons/common.h"

ProgramNode::~ProgramNode() {
    // deleteVectorMembers(imports.begin(), imports.end());
    // deleteVectorMembers(vars.begin(), vars.end());
    // deleteVectorMembers(methods.begin(), methods.end());

    auto importsIterator = imports.begin();
    while (importsIterator != imports.end()) {
        delete *importsIterator;
        importsIterator++;
    }

    auto varsIterator = vars.begin();
    while (varsIterator != vars.end()) {
        delete *varsIterator;
        varsIterator++;
    }

    auto methodsIterator = methods.begin();
    while (methodsIterator != methods.end()) {
        delete *methodsIterator;
        methodsIterator++;
    }
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

    auto importsIterator = imports.begin();
    while (importsIterator != imports.end()) {
        (*importsIterator)->print(depth+1, printTo);
        importsIterator++;
    }

    auto varsIterator = vars.begin();
    while (varsIterator != vars.end()) {
        (*varsIterator)->print(depth+1, printTo);
        varsIterator++;
    }

    auto methodsIterator = methods.begin();
    while (methodsIterator != methods.end()) {
        (*methodsIterator)->print(depth+1, printTo);
        methodsIterator++;
    }
//     printVector(imports.begin(), imports.end(), depth+1, printTo);
//     printVector(vars.begin(), vars.end(), depth+1, printTo);
//     printVector(methods.begin(), methods.end(), depth+1, printTo);
}

ProgramNode::operator string() {
    string str = "Program\n";
    // str += toString(imports.begin(), imports.end());
    // str += toString(vars.begin(), vars.end());
    // str += toString(methods.begin(), methods.end());
    return str;
}