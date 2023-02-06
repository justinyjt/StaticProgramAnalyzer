#pragma once

#include <vector>
#include "qps/clause/Clause.h"
#include "qps/pql/Synonym.h"
#include "pkb/PKBReader.h"

class QueryEvaluator {
 public:
    PKBReader* pkbReader;
    explicit QueryEvaluator(PKBReader* pkbReader);
    Result* evaluate(Synonym, std::vector<Clause*>);
};
