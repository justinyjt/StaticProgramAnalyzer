#pragma once
#include <string>
#include <vector>
#include <utility>
#include "Relationship.h"

class ModifiesS : public Relationship {
 public:
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */
    ModifiesS(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};

class ModifiesP : public Relationship {
 public:
    /* <proc SYNONYM | proc IDENT>, <SYNONYM | IDENT | _ > */
    ModifiesP(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
