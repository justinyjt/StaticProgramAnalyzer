#pragma once
#include <memory>
#include <utility>
#include "TwoArgClause.h"

class Follows : public TwoArgClause {
 public:
    /* <SYNONYM | _ | STMT_NUM> */
    const bool isTransitive;
    Follows(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, bool isTransitive);
    std::unique_ptr<Result> evaluate(PKBReader*);
    void validateArgs() override;
    bool operator==(const Clause& rhs) const;
};
