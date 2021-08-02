#ifndef SIGNATURE_H
#define SIGNATURE_H

#include "../commons/defs.h"
#include "../scanner/token.h"

class Signature {
    public:
        enum SignatureType {
            SIMPLE_VARIABLE, STRING_VARIABLE, SIMPLE_METHOD, IMPORTED_METHOD, ARRAY
        };

        // the inherent type for variables and the return type for methods
        enum DataType {
            INT, CHAR, VOID, STRING, BOOL
        };

        SignatureType type;

        DataType dataType;

        // token that represented by this Signature
        Token *token; 

        Signature();

        Signature(SignatureType type, DataType dataType, Token *token) : 
            type(type), dataType(dataType), token(token) {}

        SignatureType getSignatureType();

        DataType getDataType();

        string getSignatureTypeAsStr();

        string getDataTypeAsStr();
        
        virtual void setSignatureType(SignatureType type) = 0;

        virtual void setDataType(DataType type) = 0;

        virtual operator string() = 0;

        Token *getToken();

        void setToken(Token *token);
};

#endif 