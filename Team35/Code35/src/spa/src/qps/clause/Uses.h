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
    UsesS(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second);
    std::unique_ptr<Result> evaluate(PKBReader*);
    void validateArgs(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second);
    bool operator==(const Clause& rhs) const;
};

class UsesP : public TwoArgClause {
 public:
    /* <SYNONYM | IDENT>, <SYNONYM | _ | IDENT> */
    UsesP(PQLToken first, PQLToken second);
    std::unique_ptr<Result> evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
