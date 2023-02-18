#pragma once
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include "TwoArgClause.h"
#include "qps/result/Result.h"

class UsesS : public TwoArgClause {
 public:
    /* <SYNONYM | STMT_NUM>, <SYNONYM | _ | IDENT> */
    UsesS(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second);
    std::unique_ptr<Result> evaluate(PKBReader*) override;
    void validateArgs() override;
    bool operator==(const Clause& rhs) const override;
};
