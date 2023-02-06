#include "QPS.h"

QPS::QPS(PKBReader* pkbReader) {
    queryEvaluator = new QueryEvaluator(pkbReader);
    queryParser = new QueryParser();
};

void QPS::executeQuery(std::string& query, std::list<std::string>& result) {
    std::pair<Synonym, std::vector<Clause*>> parseResult = queryParser->parse(query);
    Result* res = queryEvaluator->evaluate(parseResult.first, parseResult.second);
    res->output(result);
}
