#pragma once

#include <memory>
#include <utility>

#include "qps/clause/OptimisableClause.h"
#include "qps/pql/Expression.h"
#include "qps/pql/Ident.h"
#include "qps/pql/PQLToken.h"
#include "qps/pql/PQLNumber.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "qps/query_exceptions/SemanticException.h"
#include "commons/types.h"

/*
Base class for any Clause with 2 arguments:
AssignPatternClause | Follows | FollowsT | Parent | ParentT | UsesS | UsesP | ModifiesS | ModifiesP | Calls | CallsT
*/
class TwoArgClause : public OptimisableClause {
 public:
    const std::unique_ptr<PQLToken> first_;
    const std::unique_ptr<PQLToken> second_;

    TwoArgClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second);

 protected:
    virtual void validateArgs() = 0;

    bool equal(const TwoArgClause &rhs) const;

    int getPairEnum() const;

    static constexpr int pairEnum(PQLToken::Tag t1, PQLToken::Tag t2) {
        return 10 * static_cast<int>(t1) + static_cast<int>(t2);
    }

    StmtType getStmtType(Synonym::DesignEntity type);
};
