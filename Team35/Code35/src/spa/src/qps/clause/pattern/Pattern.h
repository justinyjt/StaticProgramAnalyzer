#pragma once

#include <string>
#include "qps/clause/Clause.h"
#include "qps/result/Result.h"

class Pattern : public Clause {
 public:
    /* <var synonym | ident | _> , <expr | _expr_ | _> */
    Pattern(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
