#pragma once
#include <string>
#include <vector>
#include <utility>
#include "TwoArgClause.h"

class ModifiesS : public TwoArgClause {
 public:
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */
    ModifiesS(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    void validateArgs(PQLToken* first, PQLToken* second);
    bool operator==(const Clause& rhs) const;
};

class ModifiesP : public TwoArgClause {
 public:
    /* <proc SYNONYM | proc IDENT>, <SYNONYM | IDENT | _ > */
    ModifiesP(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
