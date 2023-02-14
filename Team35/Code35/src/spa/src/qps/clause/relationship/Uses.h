#pragma once
#include "Relationship.h"
#include "qps/result/Result.h"

class UsesS : public Relationship {
 public:
    /* <SYNONYM | STMT_NUM>, <SYNONYM | _ | IDENT> */
    UsesS(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};

class UsesP : public Relationship {
 public:
    /* <SYNONYM | IDENT>, <SYNONYM | _ | IDENT> */
    UsesP(PQLToken first, PQLToken second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
