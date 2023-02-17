#pragma once
#include "TwoArgClause.h"

class Parent : public TwoArgClause {
 public:
    /* <SYNONYM | _ | STMT_NUM> */
    const bool isTransitive;
    Parent(PQLToken* first, PQLToken* second, bool isTransitive);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
