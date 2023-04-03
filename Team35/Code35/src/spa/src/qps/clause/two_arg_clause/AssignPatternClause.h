#pragma once

#include <memory>
#include <utility>
#include <string>

#include "qps/clause/two_arg_clause/TwoArgClause.h"

class AssignPatternClause : public TwoArgClause {
 public:
    /* <var SYNONYM | IDENT | _> , <EXPR | _EXPR_ | _> */
    AssignPatternClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, std::string ident);

    std::unique_ptr<Result> evaluate(PKBReader *) override;

    void validateArgs() override;

    bool operator==(const Clause &rhs) const override;

 private:
    const std::string ident_;

    bool hasWildcard_ = false;

    ASSIGN_PAT_RIGHT pattern_;

    STMT_SET getStmtSet(PKBReader *db);

    STMT_ENT_SET getStmtVarSet(PKBReader *db);

    void updatePatternInfo();

    std::unique_ptr<Result> handleIdentExpr(PKBReader *db);

    std::unique_ptr<Result> handleWildCardWildCard(PKBReader *db);

    std::unique_ptr<Result> handleWildCardExpr(PKBReader *db);

    std::unique_ptr<Result> handleSynExpr(PKBReader *db);

    std::unique_ptr<Result> handleSynWildCard(PKBReader *db);

    std::unique_ptr<Result> handleIdentWildCard(PKBReader *db);
};
