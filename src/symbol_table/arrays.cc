#include "arrays.h"

#include <assert.h>

#include "../commons/common.h"

ArraySignature::ArraySignature(
    DataType dataType,
    uint32_t size,
    Token *token
) : Signature(ARRAY, dataType, token) {
    this->size = size;
}

void ArraySignature::setSignatureType(SignatureType type) {
    assert(type == ARRAY);
    this->type = type;
}

void ArraySignature::setDataType(DataType type) {
    this->dataType = type;
}

uint32_t ArraySignature::getSize() {
    return size; 
}

void ArraySignature::setSize(uint32_t size) {
    this->size = size;
}

ArraySignature::operator string() {
    assert(token != nullptr);
    return "ARRAY " + getDataTypeAsStr() + " " + token->value + "[" + std::to_string(size) + "]"; 
}
