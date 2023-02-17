#pragma once
#include "TwoArgClause.h"

class Follows : public TwoArgClause {
 public:
    /* <SYNONYM | _ | STMT_NUM> */
    const bool isRecursive;
    Follows(PQLToken* first, PQLToken* second, bool isRecursive);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
