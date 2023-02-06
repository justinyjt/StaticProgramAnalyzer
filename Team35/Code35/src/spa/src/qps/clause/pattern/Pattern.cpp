#include "Pattern.h"
#include "qps/pql/ExpressionStr.h"
#include <iostream>

Pattern::Pattern(Tok first, Tok second) : Clause(first, second) {}

// TODO SPRINT 1 
// select a pattern a ( _ , "x + 1")

Result* Pattern::evaluate(PKBReader* db) {
  // if first == "_" and second is string with no underscores
  if (first.tag == Tok::WILDCARD && second.tag == Tok::EXPR) {
    std::string pattern = static_cast<ExpressionStr&>(second).str;
    STMT_SET set = db->getStmtWithExactPatternMatch(pattern);
    IntResult* result = new IntResult(set);
    return (result);
  }
  throw std::runtime_error("");
}