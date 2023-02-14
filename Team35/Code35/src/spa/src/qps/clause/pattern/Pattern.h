#pragma once

#include <string>
#include "qps/clause/Clause.h"
#include "qps/result/Result.h"

class Pattern : public Clause {
 public:
    /* <SYNONYM | _ | IDENT>, <EXPR> */
    Pattern(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
