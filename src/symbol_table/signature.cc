#include "signature.h"

Signature::SignatureType Signature::getSignatureType() {
    return type; 
}

Signature::DataType Signature::getDataType() {
    return dataType;
}

string Signature::getSignatureTypeAsStr() {
    switch (type) {
        case SIMPLE_VARIABLE:
            return "SIMPLE_VARIABLE";
        case STRING_VARIABLE:
            return "STRING_VARIABLE";
        case SIMPLE_METHOD:
            return "SIMPLE_METHOD";
        case IMPORTED_METHOD:
            return "IMPORTED_METHOD";
        case ARRAY:
            return "ARRAY";
    }
}

string Signature::getDataTypeAsStr() {
    switch (dataType) {
        case INT:
            return "INT";
        case CHAR:
            return "CHAR";
        case VOID:
            return "VOID";
        case STRING:
            return "STRING";
        case BOOL:
            return "BOOL";
    }  
}

Token *Signature::getToken() {
    return token; 
}

void Signature::setToken(Token *token) {
    this->token = token;
}