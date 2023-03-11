#pragma once

#include <memory>
#include <utility>
#include <string>

#include "OneArgClause.h"

class WhilePattern : public OneArgClause {
    const std::string ident;
 public:
    WhilePattern(std::unique_ptr<PQLToken> first, std::string ident);
    std::unique_ptr<Result> evaluate(PKBReader *) override;
    void validateArgs() override;
    bool operator==(const Clause &rhs) const override;
};
