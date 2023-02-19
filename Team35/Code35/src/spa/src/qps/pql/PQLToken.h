#pragma once

#include <string>

/*
Base class for all PQL tokens
*/
class PQLToken {
 public:
    enum class Tag { IDENT, STMT_NUM, SYNONYM, WILDCARD, EXPR, IDENT_WILDCARD };

    explicit PQLToken(Tag tag);
    virtual ~PQLToken() = default;
    const Tag tag;

    virtual bool operator==(const PQLToken &rhs) const = 0;
    bool operator!=(const PQLToken &rhs) const;

    virtual std::string str() const = 0;

 protected:
    bool equal(const PQLToken &rhs) const;
};
