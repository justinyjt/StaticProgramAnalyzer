#pragma once
#include <memory>
#include <utility>

#include "qps/clause/Clause.h"
#include "qps/pql/Expression.h"
#include "qps/pql/Ident.h"
#include "qps/pql/PQLToken.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "qps/query_exceptions/SemanticException.h"
#include "commons/types.h"

/*
Base class for any Clause with 2 arguments:
Pattern | Follows | FollowsT | Parent | ParentT | UsesS | UsesP | ModifiesS | ModifiesP
*/
class TwoArgClause : public Clause {
 public:
    const std::unique_ptr<PQLToken> first;
    const std::unique_ptr<PQLToken> second;

    TwoArgClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second);
    bool operator==(const Clause &rhs) const override = 0;
    virtual void validateArgs() = 0;

 protected:
    bool equal(const TwoArgClause& rhs) const;
    int getPairEnum() const;
    static constexpr int pairEnum(PQLToken::Tag t1, PQLToken::Tag t2) {
      return 10 * static_cast<int>(t1) + static_cast<int>(t2);
    }
    StmtType getStmtType(Synonym::DesignEntity type);
};
