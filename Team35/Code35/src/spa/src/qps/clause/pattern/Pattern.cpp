#include "Pattern.h"
#include "qps/pql/Expression.h"

Pattern::Pattern(PQLToken* first, PQLToken* second) : Clause(first, second) {}

Result* Pattern::evaluate(PKBReader *db) {
    /* <var synonym | ident | _ , expression | partial expression | _> */

    // case: <_, expr>
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
