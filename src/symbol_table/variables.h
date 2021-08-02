#ifndef VARIABLE_SIGNATURE_H
#define VARIABLE_SIGNATURE_H

#include "signature.h"
#include "../scanner/token.h"

class VariableSignature : public Signature {
    bool isString; 

    public:
        VariableSignature(SignatureType type, DataType dataType, Token *token); 
        
        virtual void setSignatureType(SignatureType type) = 0;

        virtual void setDataType(DataType type) = 0;

        virtual operator string() = 0;
};

#endif 