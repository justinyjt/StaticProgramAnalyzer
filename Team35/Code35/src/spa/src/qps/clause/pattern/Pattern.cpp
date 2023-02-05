#include "Pattern.h"

Pattern::Pattern(EntRef& first, ExpressionSpec& second) : Clause(dynamic_cast<Arg&>(first), 
                                                              dynamic_cast<Arg&>(second)) {}

// TODO SPRINT 1 
// select a pattern a ( _ , "x + 1")

Result Pattern::evaluate() {
  // if first == "_" and second is string with no underscores
  db.getStmtWithExactPatternMatch(second);
}