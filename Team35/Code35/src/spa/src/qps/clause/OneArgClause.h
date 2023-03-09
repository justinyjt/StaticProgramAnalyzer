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
WhilePattern |
*/
class OneArgClause : public Clause {
public:
    const std::unique_ptr<PQLToken> first;

    OneArgClause(std::unique_ptr<PQLToken> first);
    virtual void validateArgs() = 0;

protected:
    bool equal(const OneArgClause& rhs) const;
    EntType getEntType(Synonym::DesignEntity type);
};
