#include "Pattern.h"
#include "qps/pql/Expression.h"

Pattern::Pattern(PQLToken* first, PQLToken* second) : Clause(first, second) {}

Result* Pattern::evaluate(PKBReader *db) {
    /* <var synonym | ident | _> , <expr | _expr_ | _> */

    switch (first->tag) {
        case PQLToken::Tag::WILDCARD: 
        {
            switch (second->tag) {
                case PQLToken::Tag::EXPR: // <_, expr> -> a(_, "x")
                {
                    std::string pattern = second->str();
                    STMT_SET set;
                    if (pattern.at(0) != '_') { // exact
                        set = db->getStmtWithExactPatternMatch(pattern);
                    } else { // partial
                        // set = db->getStmtWithPartialPatternMatch(pattern);
                    }

                    IntResult *result = new IntResult(set);
                    return result;
                }
                case PQLToken::Tag::WILDCARD: // <_, _>
                {
                    // db->getAllAssignments()
                }
            }
        }
        case PQLToken::Tag::SYNONYM:
        {
            switch (second->tag) {
                case PQLToken::Tag::EXPR: // <v, expr>
                {}
                case PQLToken::Tag::WILDCARD: // <v, _>
                {}
            }
        }
        case PQLToken::Tag::IDENT:
        {
            switch (second->tag) {
                case PQLToken::Tag::EXPR: // <ident, expr>
                {}
                case PQLToken::Tag::WILDCARD: // <ident, _>
                {}
            }
        }
    }

    throw std::runtime_error("");
}

bool Pattern::operator==(const Clause& rhs) const {
    return (dynamic_cast<const Pattern*>(&rhs) != NULL) && Clause::equal(rhs);
}
