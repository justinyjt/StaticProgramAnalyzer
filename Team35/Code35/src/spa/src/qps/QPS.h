#pragma once

#include <string>
#include <vector>
#include <utility>
#include "qps/query_parser/QueryParser.h"
#include "qps/QueryEvaluator.h"
#include "qps/clause/Clause.h"

class QPS {
 private:
    QueryParser* queryParser;
    QueryEvaluator* queryEvaluator;

 public:
    QPS(PKBReader* pkbReader);
    void executeQuery(std::string& query, std::list<std::string>& result);
};
