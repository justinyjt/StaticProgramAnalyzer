#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "qps/clause/Clause.h"
#include "qps/clause/select_clause/SelectClause.h"

class QueryEvaluator {
 public:
    explicit QueryEvaluator(PKBReader *);
    std::unique_ptr<Result> evaluate(std::pair<std::unique_ptr<SelectClause>, std::vector<std::unique_ptr<Clause>>>) const;

 private:
    PKBReader *db;
};
