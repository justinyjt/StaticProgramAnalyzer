#pragma once
#include "Relationship.h"
#include "qps/result/Result.h"

class ModifiesS : public Relationship {
 public:
    /* <SYNONYM | STMT_NUM>, <SYNONYM | _ | IDENT> */
    ModifiesS(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};

class ModifiesP : public Relationship {
 public:
    /* <SYNONYM | IDENT>, <SYNONYM | _ | IDENT> */
    ModifiesP(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
