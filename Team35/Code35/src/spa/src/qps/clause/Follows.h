#pragma once
#include "TwoArgClause.h"

class Follows : public TwoArgClause {
 public:
    /* <SYNONYM | _ | STMT_NUM> */
    const bool isTransitive;
    Follows(PQLToken* first, PQLToken* second, bool isTransitive);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
