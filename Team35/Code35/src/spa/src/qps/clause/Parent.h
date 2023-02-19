#pragma once
#include <memory>
#include <utility>

#include "TwoArgClause.h"

class Parent : public TwoArgClause {
 public:
    /* <SYNONYM | _ | STMT_NUM> */
    const bool isTransitive;
    Parent(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, bool isTransitive);
    std::unique_ptr<Result> evaluate(PKBReader *) override;
    void validateArgs() override;
    bool operator==(const Clause &rhs) const override;
};
