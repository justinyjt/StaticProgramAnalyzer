#pragma once
#include <memory>
#include <utility>
#include "TwoArgClause.h"

class ModifiesS : public TwoArgClause {
 public:
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */
    ModifiesS(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second);
    std::unique_ptr<Result> evaluate(PKBReader*) override;
    void validateArgs() override;
    bool operator==(const Clause& rhs) const override;
};
