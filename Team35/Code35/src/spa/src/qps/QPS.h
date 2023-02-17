#pragma once

#include <memory>
#include <list>
#include <string>
#include <vector>
#include <utility>
#include "pkb/PKBReader.h"
#include "commons/lexer/Lexer.h"
#include "qps/query_parser/QueryParser.h"
#include "qps/clause/Clause.h"
#include "qps/QueryEvaluator.h"

class QPS {
 private:
    QueryEvaluator* queryEvaluator;
    QueryParser* queryParser;

 public:
    explicit QPS(PKBReader* pkbReader);
    void executeQuery(std::string& query, std::list<std::string>& result);
};
