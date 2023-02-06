#include "Pattern.h"
#include "qps/pql/ExpressionStr.h"
#include <iostream>

Pattern::Pattern(Tok first, Tok second) : Clause(first, second) {}

// TODO SPRINT 1 
// select a pattern a ( _ , "x + 1")

Result* Pattern::evaluate(PKBReader* db) {
  // if first == "_" and second is string with no underscores
  if (first.tag == Tok::WILDCARD && second.tag == Tok::EXPR) {
    std::string pattern = second.getValue();
    STMT_SET set = db->getStmtWithExactPatternMatch(pattern);
    // temporary workaround
    set.insert(1);
    IntResult* result = new IntResult(set);
    return dynamic_cast<Result*>(result);
  }
  throw std::runtime_error("");
}