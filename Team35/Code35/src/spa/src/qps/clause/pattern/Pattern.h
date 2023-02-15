#pragma once

#include <string>
#include <vector>
#include <utility>
#include <unordered_set>
#include "qps/clause/Clause.h"
#include "qps/result/Result.h"

class Pattern : public Clause {
    const std::string ident;
 public:
    /* <var SYNONYM | IDENT | _> , <EXPR | _EXPR_ | _> */
    Pattern(PQLToken* first, PQLToken* second, std::string ident);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
