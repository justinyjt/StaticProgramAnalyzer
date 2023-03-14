#pragma once

#include <memory>
#include <utility>

#include "TwoArgClause.h"

class WithEntClause : public TwoArgClause {
 public:
    WithEntClause(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>);
    std::unique_ptr<Result> evaluate(PKBReader *) override;
    void validateArgs() override;
 protected:
    bool operator==(const Clause& rhs) const override;
};
