#include "QPS.h"

QPS::QPS(PKBReader* pkbReader) {
  queryEvaluator = new QueryEvaluator(pkbReader);
  queryParser = new QueryParser();
};

void QPS::executeQuery(std::string& query, std::list<std::string>& result) {
  std::vector<Clause*> clauses = queryParser->parse(query);
  Result* eval = queryEvaluator->evaluate(clauses);
  eval->output(result);
}
