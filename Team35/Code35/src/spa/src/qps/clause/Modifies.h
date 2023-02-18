#pragma once

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include "TwoArgClause.h"

class ModifiesS : public TwoArgClause {
 public:
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */
    ModifiesS(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second);
    std::unique_ptr<Result> evaluate(PKBReader*);
    void validateArgs(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second);
    bool operator==(const Clause& rhs) const;
};

class ModifiesP : public TwoArgClause {
 public:
    /* <proc SYNONYM | proc IDENT>, <SYNONYM | IDENT | _ > */
    ModifiesP(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second);
    std::unique_ptr<Result> evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
