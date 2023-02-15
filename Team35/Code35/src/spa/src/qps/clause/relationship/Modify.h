#pragma once
#include "Relationship.h"
#include "qps/result/Result.h"

class ModifiesS : public Relationship {
 public:
    /* <SYNONYM | STMT_NUM>, <SYNONYM | WILDCARD | IDENT_STR> */
    ModifiesS(Tok* first, Tok* second);
    Result* evaluate(PKBReader*);
    void validateArgs(Tok* first, Tok* second);
    bool operator==(const Clause& rhs) const;
};
