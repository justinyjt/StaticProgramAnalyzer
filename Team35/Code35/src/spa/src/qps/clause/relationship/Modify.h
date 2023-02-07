#pragma once
#include "Relationship.h"
#include "qps/result/Result.h"

class Modify : public Relationship {
 public:
    /* <SYNONYM | STMT_NUM>, <SYNONYM | WILDCARD | IDENT_STR> */
    Modify(Tok* first, Tok* second);
    Result* evaluate(PKBReader*);

    bool operator==(Modify const &rhs) const;
};

class ModifyS : public Relationship {
 public:
    /* <SYNONYM | IDENT_STR>, <SYNONYM | WILDCARD | IDENT_STR> */
    ModifyS(Tok first, Tok second);
};
