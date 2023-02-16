#pragma once
#include <string>
#include <vector>
#include <utility>
#include <unordered_set>
#include "TwoArgClause.h"

class Follows : public TwoArgClause {
 public:
    /* <SYNONYM | _ | STMT_NUM> */
    const bool isRecursive;
    Follows(PQLToken* first, PQLToken* second, bool isRecursive);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
