#pragma once

#include <string>

/*
Base class for all PQL tokens
*/
class Tok {
 public:
    enum Tag { IDENT, STMT_NUM, SYNONYM, WILDCARD, EXPR };

    Tok(Tag tag);
    const Tag tag;

    bool operator==(const Tok &rhs) const;

    virtual std::string str() const = 0; };
