#pragma once

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <list>
#include <unordered_set>
#include "TwoArgClause.h"

class Parent : public TwoArgClause {
 public:
    /* <SYNONYM | _ | STMT_NUM> */
    const bool isRecursive;
    Parent(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, bool isRecursive);
    std::unique_ptr<Result> evaluate(PKBReader*) override;
    void validateArgs() override;
    bool operator==(const Clause& rhs) const override;
};
