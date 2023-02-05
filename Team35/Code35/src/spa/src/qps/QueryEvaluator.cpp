#include "qps/QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKB& pkb) : pkb(pkb) {
  pkbReader = new PKBReader(pkb);
}

Result QueryEvaluator::evaluate(Synonym syn, std::vector<Clause> clauses) {
  std::vector<Result> results;

  for (Clause cl: clauses) {
    cl.evaluate(pkbReader);
  }

  if (clauses.size() == 1) {
    return results.at(0);
  }
  // TODO join results for > 1 clauses
  Result r;
  return r;

}
