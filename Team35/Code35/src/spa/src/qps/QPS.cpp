#include "QPS.h"

QPS::QPS(PKBReader* pkbReader) {
    queryEvaluator = new QueryEvaluator(pkbReader);
    queryParser = new QueryParser();
};

void QPS::executeQuery(std::string& query, std::list<std::string>& result) {
    // Perform parsing
    std::pair<Synonym, std::vector<Clause*>> parseResult = queryParser->parse(query);

    // Perform evaluation
    Result* res = queryEvaluator->evaluate(parseResult.first, parseResult.second);

    // store in result list
    res->output(result);
}
