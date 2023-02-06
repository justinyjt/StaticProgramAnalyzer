#pragma once
#include <string>
#include "Tok.h"

/*
represents **declared** synonym 
*/
class Synonym : public Tok {
public:
    enum DesignEntity {
        STMT, READ, PRINT, CALL, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROCEDURE
    };

    Synonym(DesignEntity de, std::string ident) : Tok::Tok(ident,  SYNONYM), de(de) {};

    bool operator==(const Synonym &rhs) const {
        return this->de == rhs.de && this->value == rhs.value;
    }

    const DesignEntity de;
};
