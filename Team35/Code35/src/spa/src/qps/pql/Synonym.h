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

    Synonym(DesignEntity de, std::string ident);

    bool operator==(const Synonym &rhs) const;

    const DesignEntity de;
};
