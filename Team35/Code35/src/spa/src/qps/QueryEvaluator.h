#pragma once

#include <string>
#include <vector>
#include "qps/clause/Clause.h"

class QueryEvaluator {
 private:
    PKBReader* db;

 public:
   QueryEvaluator(PKBReader*);
   Result* evaluate(std::vector<Clause*>) const;
};
