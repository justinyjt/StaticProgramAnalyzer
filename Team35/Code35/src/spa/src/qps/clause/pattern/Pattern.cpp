#include "Pattern.h"
#include "qps/pql/ExpressionStr.h"

Pattern::Pattern(Tok* first, Tok* second) : Clause(first, second) {
    validateArgs(first, second);
}

// select a pattern a ( _ , "x + 1")

// LHS - variable synonyms

Result* Pattern::evaluate(PKBReader *db) {
    // if first == "_" and second is string with no underscores
    if (first->tag == Tok::Tag::WILDCARD && second->tag == Tok::Tag::EXPR) {
        std::string pattern = second->str();
        STMT_SET set = db->getStmtWithExactPatternMatch(pattern);

        IntResult *result = new IntResult(set);
        return dynamic_cast<Result *>(result);
    }
    throw std::runtime_error("");
}

void Pattern::validateArgs(Tok* first, Tok* second) {
}

bool Pattern::operator==(const Clause& rhs) const {
    return (dynamic_cast<const Pattern*>(&rhs) != NULL) && Clause::equal(rhs);
}
