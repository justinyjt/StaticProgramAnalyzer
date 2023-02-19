#pragma once

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "qps/clause/Clause.h"

class QueryEvaluator {
 private:
    PKBReader* db;

 public:
    explicit QueryEvaluator(PKBReader*);
    std::unique_ptr<Result> evaluate(std::vector<std::unique_ptr<Clause>>&) const;
};
