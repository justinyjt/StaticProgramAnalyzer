#pragma once
#include "Relationship.h"
#include "qps/result/Result.h"

class ModifiesS : public Relationship {
 public:
    /* <stmt synonym | stmt_num>, <var synonym | ident | _ > */
    ModifiesS(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};

class ModifiesP : public Relationship {
 public:
    /* <proc synonym | proc ident>, <synonym | ident | _ > */
    ModifiesP(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
