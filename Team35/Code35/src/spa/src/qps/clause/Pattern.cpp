#include "Pattern.h"
#include "qps/pql/Expression.h"

Pattern::Pattern(PQLToken* first, PQLToken* second, std::string ident) : TwoArgClause(first, second), ident(ident) {}

Result* Pattern::evaluate(PKBReader *db) {
    /* <var SYNONYM | IDENT | _> , <EXPR | _EXPR_ | _> */
    STMT_SET stmtSet2;

    if (second->tag == PQLToken::Tag::EXPR) {
        std::string pattern = second->str();
        if (pattern.at(0) != '_') {  // exact
            stmtSet2 = db->getStmtWithExactPatternMatch(pattern);
        } else {  // partial
            stmtSet2 = db->getStmtWithPartialPatternMatch(pattern);
        }
    }

    switch (caseValue()) {
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::EXPR):  // a(_, "x") -> int[]
        {
            Result *result = new OneColResult<int>(this->ident, stmtSet2);
            return result;
        }
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // (_, _) -> int[]
        {
            STMT_ENT_SET stmtEntSet = db->getAllRelationships(StmtNameRelationship::Modifies);
            STMT_SET stmtSetResult;
            for (STMT_ENT se : stmtEntSet)
                stmtSetResult.insert(se.first);
            Result *result = new OneColResult<int>(this->ident, stmtSetResult);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::EXPR):  // a(v, "_x_") -> pair<int, str>[]
        {
            std::string synonymIdent = (dynamic_cast<const Synonym*>(first))->ident;
            std::vector<std::pair<int, std::string>> vec;
            for (STMT_NUM s : stmtSet2) {  // for each statement, find e that is modified
                ENT_SET entSet = db->getRelationship(StmtNameRelationship::Modifies, s);
                for (const std::string& ent : entSet)
                    vec.emplace_back(s, ent);
            }
            Result *result = new TwoColResult<int, std::string>(this->ident, synonymIdent, vec);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // a(v, _) -> pair<int, str>[]
        {
            std::string synonymIdent = (dynamic_cast<const Synonym*>(first))->ident;
            STMT_ENT_SET stmtEntSet = db->getAllRelationships(StmtNameRelationship::Modifies);
            std::vector<std::pair<int, std::string>> vec;
            for (auto& se : stmtEntSet)
                vec.push_back(se);
            Result *result = new TwoColResult<int, std::string>(this->ident, synonymIdent, vec);
            return result;
        }
        case c(PQLToken::Tag::IDENT, PQLToken::Tag::EXPR):  // a("x", "_1_") -> int[]
        {
            STMT_SET stmtSet1 = db->getRelationship(StmtNameRelationship::Modifies, first->str());
            STMT_SET stmtSetResult;
            for (STMT_NUM s : stmtSet1) {  // find s1 n s2
                if (stmtSet2.find(s) != stmtSet2.end()) {
                    stmtSetResult.insert(s);
                }
            }
            Result* result = new OneColResult<int>(this->ident, stmtSetResult);
            return result;
        }
        case c(PQLToken::Tag::IDENT, PQLToken::Tag::WILDCARD):  // a("x", _) -> int[]
        {
            STMT_SET stmtSet1 = db->getRelationship(StmtNameRelationship::Modifies, first->str());
            Result *result = new OneColResult<int>(this->ident, stmtSet1);
            return result;
        }
        default:
            throw std::runtime_error("Pattern.cpp");
    }
}

bool Pattern::operator==(const Clause& rhs) const {
    const Pattern* pRhs = dynamic_cast<const Pattern*>(&rhs);
    if (pRhs != nullptr) {
        return ident == pRhs->ident && equal(*pRhs);
    }
    return false;
}
