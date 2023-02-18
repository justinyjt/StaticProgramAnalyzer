#include "QPS.h"
#include "qps/clause/SelectClause.h"

QPS::QPS(PKBReader* pkbReader) {
  queryEvaluator = new QueryEvaluator(pkbReader);
  queryParser = new QueryParser();
};

void QPS::executeQuery(std::string& query, std::list<std::string>& result) {
  std::vector<Clause*> clauses = queryParser->parse(query);
  std::string selected = dynamic_cast<SelectClause*>(clauses[0])->syn.ident;
  Result* eval = queryEvaluator->evaluate(clauses);
  eval->output(result, selected);
}
