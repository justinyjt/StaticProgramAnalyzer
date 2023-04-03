#pragma once
#include <memory>
#include <utility>
#include <string>

#include "qps/clause/two_arg_clause/TwoArgClause.h"

class AssignPatternClause : public TwoArgClause {
    const std::string ident_;
 public:
    /* <var SYNONYM | IDENT | _> , <EXPR | _EXPR_ | _> */
    AssignPatternClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, std::string ident);
    std::unique_ptr<Result> evaluate(PKBReader *) override;
    void validateArgs() override;
    bool operator==(const Clause &rhs) const override;
};
