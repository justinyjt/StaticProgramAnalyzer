#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "qps/clause/OptimisableClause.h"
#include "qps/clause/select_clause/SelectClause.h"

class QueryEvaluator {
 public:
    explicit QueryEvaluator(PKBReader *);
    std::unique_ptr<Result> evaluate(std::unique_ptr<SelectClause> selectClause,
                                     std::vector<std::unique_ptr<OptimisableClause>> clauses) const;

 private:
    PKBReader *db;
};
