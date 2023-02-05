#include <string>
#include "Tok.h"
#include "Arg.h"

/*
represents **declared** synonym 
*/
class Synonym : public Tok, public StmtRef, public EntRef {
public:
    enum DesignEntity {
        STMT, READ, PRINT, CALL, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROCEDURE
    };

    Synonym(DesignEntity de, std::string ident) : de(de), ident(ident) {};

    const DesignEntity de;
    const std::string ident;
};
