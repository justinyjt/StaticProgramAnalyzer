#pragma once

#include <string>
#include <list>
#include <memory>
#include <vector>
#include <utility>
#include <unordered_set>
#include "qps/clause/TwoArgClause.h"
#include "qps/result/Result.h"

class Pattern : public TwoArgClause {
    const std::string ident;
 public:
    /* <var SYNONYM | IDENT | _> , <EXPR | _EXPR_ | _> */
    Pattern(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second, std::string ident);
    std::unique_ptr<Result> evaluate(PKBReader*);
    void validateArgs(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second);
    bool operator==(const Clause& rhs) const;
};
