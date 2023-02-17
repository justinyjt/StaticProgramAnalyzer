#pragma once

#include <string>
#include "qps/clause/TwoArgClause.h"

class Pattern : public TwoArgClause {
    const std::string ident;
 public:
    /* <var SYNONYM | IDENT | _> , <EXPR | _EXPR_ | _> */
    Pattern(PQLToken* first, PQLToken* second, std::string ident);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
