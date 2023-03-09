#pragma once
#include <memory>
#include <utility>
#include <string>

#include "qps/clause/TwoArgClause.h"

class AssignPattern : public TwoArgClause {
    const std::string ident;
 public:
    /* <var SYNONYM | IDENT | _> , <EXPR | _EXPR_ | _> */
    AssignPattern(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, std::string ident);
    std::unique_ptr<Result> evaluate(PKBReader *) override;
    void validateArgs() override;
    bool operator==(const Clause &rhs) const override;
};
