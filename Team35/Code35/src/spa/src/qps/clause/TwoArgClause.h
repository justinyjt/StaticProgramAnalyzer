#pragma once
#include <memory>
#include "qps/clause/Clause.h"
#include "commons/types.h"

/*
Base class for any Clause with 2 arguments:
Pattern | Follows | FollowsT | Parent | ParentT | UsesS | UsesP | ModifiesS | ModifiesP
*/
class TwoArgClause : public Clause {
 public:
    const std::shared_ptr<PQLToken> first;
    const std::shared_ptr<PQLToken> second;

    TwoArgClause(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second);
    virtual bool operator==(const Clause &rhs) const = 0;

 protected:
    bool equal(const TwoArgClause& rhs) const;
    int caseValue() const;
    static constexpr int c(PQLToken::Tag t1, PQLToken::Tag t2) {
      return 10 * static_cast<int>(t1) + static_cast<int>(t2);
    }
};
