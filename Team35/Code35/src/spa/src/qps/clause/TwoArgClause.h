#pragma once
#include "qps/clause/Clause.h"
#include "qps/pql/Expression.h"
#include "qps/pql/Ident.h"
#include "qps/pql/PQLToken.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "commons/types.h"

/*
Base class for any Clause with 2 arguments:
Pattern | Follows | FollowsT | Parent | ParentT | UsesS | UsesP | ModifiesS | ModifiesP
*/
class TwoArgClause : public Clause {
 public:
    const PQLToken* first;
    const PQLToken* second;

    TwoArgClause(PQLToken* first, PQLToken* second);
    virtual bool operator==(const Clause &rhs) const = 0;

 protected:
    bool equal(const TwoArgClause& rhs) const;
    int getPairEnum() const;
    static constexpr int pairEnum(PQLToken::Tag t1, PQLToken::Tag t2) {
      return 10 * static_cast<int>(t1) + static_cast<int>(t2);
    }
};
