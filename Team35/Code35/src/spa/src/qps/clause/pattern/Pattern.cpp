#include "Pattern.h"
#include "qps/pql/ExpressionStr.h"

Pattern::Pattern(PQLToken* first, PQLToken* second) : Clause(first, second) {}

// select a pattern a ( _ , "x + 1")

Result* Pattern::evaluate(PKBReader *db) {
    // if first == "_" and second is string with no underscores
    if (first->tag == PQLToken::Tag::WILDCARD && second->tag == PQLToken::Tag::EXPR) {
        std::string pattern = second->str();
        STMT_SET set = db->getStmtWithExactPatternMatch(pattern);

        IntResult *result = new IntResult(set);
        return result;
    }
    throw std::runtime_error("");
}

bool Pattern::operator==(const Clause& rhs) const {
    return (dynamic_cast<const Pattern*>(&rhs) != NULL) && Clause::equal(rhs);
}
