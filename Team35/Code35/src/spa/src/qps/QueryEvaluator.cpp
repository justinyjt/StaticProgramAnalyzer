#include "qps/QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKBReader& pkbReader) : pkbReader(pkbReader) {};

Result* QueryEvaluator::evaluate(Synonym syn, std::vector<Clause*> clauses) {
  std::vector<Result*> results;

  for (Clause* cl: clauses) {
    results.push_back(cl->evaluate(&pkbReader));
  }

  if (clauses.size() == 1) {
    return results.at(0);
  }
  // TODO join results for > 1 clauses
  return results.at(0);

}
