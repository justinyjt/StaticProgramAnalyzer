#pragma once
#include "TwoArgClause.h"

class UsesS : public TwoArgClause {
 public:
    /* <SYNONYM | STMT_NUM>, <SYNONYM | _ | IDENT> */
    UsesS(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};

class UsesP : public TwoArgClause {
 public:
    /* <SYNONYM | IDENT>, <SYNONYM | _ | IDENT> */
    UsesP(PQLToken first, PQLToken second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
