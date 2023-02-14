#pragma once
#include "qps/clause/Clause.h"
#include "commons/types.h"

/*
Base class for any Relationship:
Follows | FollowsT | Parent | ParentT | UsesS | UsesP | ModifiesS | ModifiesP
*/
class Relationship : public Clause {
 public:
        Relationship(PQLToken* first, PQLToken* second);
        virtual Result* evaluate(PKBReader* db) = 0;
};
