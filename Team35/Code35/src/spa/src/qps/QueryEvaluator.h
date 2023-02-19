#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "qps/clause/Clause.h"

class QueryEvaluator {
 public:
    explicit QueryEvaluator(PKBReader *);
    std::unique_ptr<Result> evaluate(std::vector<std::unique_ptr<Clause>> &) const;

 private:
    PKBReader *db;
};
