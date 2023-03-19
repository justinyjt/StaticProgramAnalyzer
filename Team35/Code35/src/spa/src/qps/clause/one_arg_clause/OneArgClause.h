#pragma once

#include <memory>
#include <utility>
#include <string>

#include "qps/clause/Clause.h"
#include "qps/pql/Expression.h"
#include "qps/pql/Ident.h"
#include "qps/pql/PQLToken.h"
#include "qps/pql/PQLNumber.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "qps/query_exceptions/SemanticException.h"
#include "commons/types.h"

/*
Base class for any Clause with 1 arguments:
IfPattern | WhilePattern
*/
class OneArgClause : public Clause {
 public:
    const std::unique_ptr<PQLToken> first;

    const std::string ident;

    OneArgClause(std::unique_ptr<PQLToken> first, StmtNameRelationship rs, std::string ident);

    std::unique_ptr<Result> evaluate(PKBReader *) override;

    bool operator==(const Clause& rhs) const override;

 protected:
    void validateArgs();

    StmtType getStmtType() const;

    StmtNameRelationship rs;
};

class IfPattern : public OneArgClause {
 public:
    IfPattern(std::unique_ptr<PQLToken> arg, std::string ident);
};

class WhilePattern : public OneArgClause {
 public:
    WhilePattern(std::unique_ptr<PQLToken> arg, std::string ident);
};
