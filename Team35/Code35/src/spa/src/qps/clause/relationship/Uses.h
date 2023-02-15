#pragma once

#include "qps/result/Result.h"
#include "qps/pql/Tok.h"
#include "qps/clause/Clause.h"
#include "Relationship.h"

class UsesS : public Relationship {
 public:
    UsesS(Tok* first, Tok* second);
    Result* evaluate(PKBReader*);
    void validateArgs(Tok* first, Tok* second);
    bool operator==(const Clause& rhs) const;
};
