#pragma once

#include <string>
#include "qps/clause/Clause.h"
#include "qps/result/Result.h"

class Pattern : public Clause {
 public:
    /* <SYNONYM | WILDCARD | IDENT_STR>, <EXPR_STR> */
    Pattern(Tok* first, Tok* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
