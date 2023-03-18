#pragma once
#include <memory>
#include <utility>
#include <string>

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
Base class for any Clause with 1 arguments:
IfPattern | WhilePattern
*/
class OneArgClause : public Clause {
 public:
    const std::unique_ptr<PQLToken> first;
    std::string ident;

    explicit OneArgClause(std::unique_ptr<PQLToken> first, StmtNameRelationship rs, std::string ident);
    virtual void validateArgs() = 0;
    std::unique_ptr<Result> evaluate(PKBReader *) override;

 protected:
    bool equal(const OneArgClause& rhs) const;
    StmtNameRelationship rs;
};

class IfPattern : public OneArgClause {
 public:
    IfPattern(std::unique_ptr<PQLToken> arg, std::string ident);
    void validateArgs() override;
    bool operator==(const Clause &rhs) const override;
};

class WhilePattern : public OneArgClause {
 public:
    WhilePattern(std::unique_ptr<PQLToken> arg, std::string ident);
    void validateArgs() override;
    bool operator==(const Clause &rhs) const override;
};
