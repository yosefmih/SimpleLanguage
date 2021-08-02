#ifndef METHOD_SIGNATURE_H
#define METHOD_SIGNATURE_H

#include <vector>

#include "signature.h"
#include "../commons/defs.h"

class MethodSignature : public Signature {
    bool imported;
    std::vector<Signature*> args;

    public:
        MethodSignature(SignatureType type, DataType dataType, Token *token);

        ~MethodSignature();
        
        virtual void setSignatureType(SignatureType type) = 0;

        virtual void setDataType(DataType type) = 0;

        virtual operator string() = 0;

        std::vector<Signature*>& getArgs();

        uint32_t getNumArgs();

        void addArg(Signature *arg);

        bool isImported();
};

#endif 