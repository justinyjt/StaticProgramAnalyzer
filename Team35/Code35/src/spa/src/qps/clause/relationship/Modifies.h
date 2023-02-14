#pragma once
#include "Relationship.h"
#include "qps/result/Result.h"

class ModifiesS : public Relationship {
 public:
    /* <SYNONYM | STMT_NUM>, <SYNONYM | WILDCARD | IDENT_STR> */
    ModifiesS(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};

class ModifiesP : public Relationship {
 public:
    /* <SYNONYM | IDENT_STR>, <SYNONYM | WILDCARD | IDENT_STR> */
    ModifiesP(PQLToken first, PQLToken second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
