#include "variables.h"

#include <assert.h>

#include "../commons/exceptions.h"

VariableSignature::VariableSignature(
    SignatureType type,
    DataType dataType,
    Token *token
) : Signature(type, dataType, token) {
    assert(type == STRING_VARIABLE || type == SIMPLE_VARIABLE);
    
    if (type == STRING_VARIABLE) {
        isString = true;
    } else {
        isString = false;
    }

    if (type == STRING_VARIABLE && dataType != STRING) {
        throw Exception("STRING_VARIABLE must have STRING type");
    }

    if (this->type == STRING_VARIABLE && type != STRING) {
        throw Exception("STRING_VARIABLE must have STRING type");
    }
}

void VariableSignature::setSignatureType(SignatureType type) {
    assert(type == STRING_VARIABLE || type == SIMPLE_VARIABLE);
    if (type == STRING_VARIABLE) {
        isString = true;
    } else {
        isString = false;
    }
    this->type = type;
}

void VariableSignature::setDataType(DataType type) {
    if (this->type == STRING_VARIABLE && type != STRING) {
        throw Exception("STRING_VARIABLE must have STRING type");
    }
    dataType = type;
}

VariableSignature::operator string() {
    assert(token != nullptr);
    return getSignatureTypeAsStr() + " " + getDataTypeAsStr() + " " + token->value;
}