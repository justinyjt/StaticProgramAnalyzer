#pragma once
#include "Relationship.h"
#include "qps/result/Result.h"

class Parent : public Relationship {
 public:
    /* <SYNONYM | _ | STMT_NUM> */
    Parent(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};

class ParentP : public Relationship {
 public:
    /* <SYNONYM | _ | STMT_NUM> */
    ParentP(PQLToken first, PQLToken second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
