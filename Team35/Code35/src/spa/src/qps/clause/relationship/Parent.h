#pragma once

#include "qps/result/Result.h"
#include "qps/pql/Tok.h"
#include "qps/clause/Clause.h"
#include "Relationship.h"

class Parent : public Relationship {
 public:
    Parent(Tok* first, Tok* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};

class ParentT : public Relationship {
 public:
    ParentT(Tok* first, Tok* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
