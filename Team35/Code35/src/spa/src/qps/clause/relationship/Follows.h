#pragma once
#include "Relationship.h"
#include "qps/result/Result.h"

class Follows : public Relationship {
 public:
    /* <SYNONYM | _ | STMT_NUM> */
    Follows(PQLToken* first, PQLToken* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};

class FollowsT : public Relationship {
 public:
    /* <SYNONYM | _ | STMT_NUM > */ 
    FollowsT(PQLToken first, PQLToken second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
