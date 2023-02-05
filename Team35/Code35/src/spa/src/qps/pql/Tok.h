#pragma once

/*
Base class for all PQL tokens
*/
class Tok {
  public:
    enum Tag { IDENT, STMT_NUM, SYNONYM, WILDCARD, EXPR };

    Tok(Tag tag) : tag(tag) {};
    const Tag tag;

    bool operator==(const Tok &rhs) const {
        return this->tag == rhs.tag;
    }
};