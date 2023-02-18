#pragma once

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <list>
#include <unordered_set>
#include "TwoArgClause.h"

class Follows : public TwoArgClause {
 public:
    /* <SYNONYM | _ | STMT_NUM> */
    const bool isRecursive;
    Follows(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, bool isRecursive);
    std::unique_ptr<Result> evaluate(PKBReader*);
    void validateArgs() override;
    bool operator==(const Clause& rhs) const;
};
