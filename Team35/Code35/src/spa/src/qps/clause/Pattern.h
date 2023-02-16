#pragma once

#include <string>
#include <list>
#include <vector>
#include <utility>
#include <unordered_set>
#include "qps/clause/TwoArgClause.h"
#include "qps/result/Result.h"

class Pattern : public TwoArgClause {
    const std::string ident;
 public:
    /* <var SYNONYM | IDENT | _> , <EXPR | _EXPR_ | _> */
    Pattern(PQLToken* first, PQLToken* second, std::string ident);
    Result* evaluate(PKBReader*);
    void validateArgs(PQLToken* first, PQLToken* second);
    bool operator==(const Clause& rhs) const;
};
