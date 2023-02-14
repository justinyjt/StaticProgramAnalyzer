#pragma once
#include <string>
#include "Tok.h"

/*
represents **declared** synonym 
*/
class Synonym : public Tok {
 public:
    enum class DesignEntity {
        STMT, READ, PRINT, CALL, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROCEDURE
    };

    Synonym(DesignEntity de, std::string ident);

    bool operator==(const Tok& rhs) const;
    std::string str() const;

    const DesignEntity de;
    const std::string ident;
};
