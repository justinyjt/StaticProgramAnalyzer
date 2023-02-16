#pragma once
#include <string>
#include <vector>
#include <utility>
#include <unordered_set>
#include "TwoArgClause.h"

class Parent : public TwoArgClause {
 public:
    /* <SYNONYM | _ | STMT_NUM> */
    const bool isRecursive;
    Parent(PQLToken* first, PQLToken* second, bool isRecursive);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
