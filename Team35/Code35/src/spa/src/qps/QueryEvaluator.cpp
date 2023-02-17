#pragma once
#include "qps/QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKBReader* db) : db(db) {}

Result* QueryEvaluator::evaluate(std::vector<Clause*> clauses) const {
  // first clause
  Result* curr = clauses[0]->evaluate(db);
  int i = 1;
  while (i < clauses.size()) {
      Result* next = clauses[i]->evaluate(db);
      Result* merged = Result::join(curr, next);
      delete curr;
      delete next;
      curr = merged;
  }
  return curr;
}
