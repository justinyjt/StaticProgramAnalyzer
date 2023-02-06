#pragma once

#include <string>

/*
Base class for all PQL tokens
*/
class Tok {
 public:
    enum Tag { IDENT, STMT_NUM, SYNONYM, WILDCARD, EXPR };

    Tok(std::string value, Tag tag);
    const Tag tag;
    const std::string value;

    bool operator==(const Tok &rhs) const;

    std::string getValue();
};
