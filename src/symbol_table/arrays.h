#ifndef ARRAY_SIGNATURE_H
#define ARRAY_SIGNATURE_H

#include "signature.h"
#include "../scanner/token.h"
#include "../commons/defs.h"

class ArraySignature : public Signature {
    uint32_t size;

    public:
        ArraySignature(DataType dataType, uint32_t size, Token *token);

        virtual void setSignatureType(SignatureType type) = 0;

        virtual void setDataType(DataType type) = 0;

        virtual operator string() = 0;

        uint32_t getSize();

        void setSize(uint32_t size);
};

#endif 