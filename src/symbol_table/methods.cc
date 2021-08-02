#include "methods.h"

#include <assert.h>

#include "../commons/exceptions.h"

MethodSignature::MethodSignature(
    SignatureType type,
    DataType dataType, 
    Token *token
) : Signature(type, dataType, token) {
    if (type == IMPORTED_METHOD) {
        imported = true;
    } else if (type == SIMPLE_METHOD) {
        imported = false;
    } else {
        throw Exception("MethodSignature must have type IMPORTED_METHOD or SIMPLE_METHOD");
    }
}

MethodSignature::~MethodSignature() {
    auto argIterator = args.begin();
    while (argIterator != args.end()) {
        if (*argIterator) {
            delete *argIterator;
        }
        argIterator++;
    }
}

void MethodSignature::setSignatureType(SignatureType type) {
    if (type == SIMPLE_METHOD) {
        imported = false;
    } else if (type == IMPORTED_METHOD) {
        imported = true;
    } else {
        throw Exception("MethodSignature must have type IMPORTED_METHOD or SIMPLE_METHOD");
    }
    this->type = type; 
}

void MethodSignature::setDataType(DataType type) {
    dataType = type; 
}

// (IMPORTED METHOD | SIMPLE METHOD) NAME_OF_METHOD (COMMA SEPARATED LIST OF ARGS)
MethodSignature::operator string() {
    assert(token != nullptr);
    string str = isImported ? "IMPORTED METHOD " : "SIMPLE METHOD ";
    str += token->value + "("; 
    auto argIterator = args.begin();
    while (argIterator != args.end()) {
        str += ", " + string(**argIterator);
        argIterator++;
    }
    str += ")";
    return str;
}

std::vector<Signature*>& MethodSignature::getArgs() {
    return args;
}

uint32_t MethodSignature::getNumArgs() {
    return args.size();
}

void MethodSignature::addArg(Signature *arg) {
    args.push_back(arg);
}

bool MethodSignature::isImported() {
    return imported;
}