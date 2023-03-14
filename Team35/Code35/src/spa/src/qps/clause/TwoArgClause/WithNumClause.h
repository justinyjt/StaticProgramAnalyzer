#pragma once

#include <memory>
#include <utility>

#include "TwoArgClause.h"

class WithNumClause : public TwoArgClause {
public:
    WithNumClause(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>);
    std::unique_ptr<Result> evaluate(PKBReader *) override;
    void validateArgs() override;
protected:
    bool operator==(const Clause& rhs) const override;
};
