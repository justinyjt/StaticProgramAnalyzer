#include "Pattern.h"
#include "qps/pql/ExpressionStr.h"

Pattern::Pattern(Tok* first, Tok* second) : Clause(first, second) {}

// select a pattern a ( _ , "x + 1")

Result *Pattern::evaluate(PKBReader *db) {
    // if first == "_" and second is string with no underscores
    if (first->tag == Tok::WILDCARD && second->tag == Tok::EXPR) {
        std::string pattern = second->str();
        STMT_SET set = db->getStmtWithExactPatternMatch(pattern);

        IntResult *result = new IntResult(set);
        return dynamic_cast<Result *>(result);
    }
    throw std::runtime_error("");
}
