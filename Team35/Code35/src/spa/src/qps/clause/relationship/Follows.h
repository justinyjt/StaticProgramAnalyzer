#pragma once

#include "Relationship.h"
#include "qps/result/Result.h"

class Follows : public Relationship {
 public:
    Follows(Tok* first, Tok* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};

class FollowsT : public Relationship {
 public:
    FollowsT(Tok* first, Tok* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
