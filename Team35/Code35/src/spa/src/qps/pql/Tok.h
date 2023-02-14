#pragma once

#include <string>

/*
Base class for all PQL tokens
*/
class Tok {
 public:
    enum class Tag { IDENT, STMT_NUM, SYNONYM, WILDCARD, EXPR, IDENT_WILDCARD };

    explicit Tok(Tag tag);
    const Tag tag;

    virtual bool operator==(const Tok &rhs) const = 0;
    bool operator!=(const Tok &rhs) const;

    virtual std::string str() const = 0;

 protected:
    bool equal(const Tok &rhs) const;
};
