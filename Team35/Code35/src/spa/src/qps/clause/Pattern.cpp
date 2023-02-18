#include "Pattern.h"
#include "qps/pql/Expression.h"
#include "qps/query_exceptions/SemanticException.h"

Pattern::Pattern(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second,
                 std::string ident) : TwoArgClause(first, second), ident(ident) {
    validateArgs(first, second);
}

std::unique_ptr<Result> Pattern::evaluate(PKBReader *db) {
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
            std::unique_ptr<Result> result = std::make_unique<TableResult>(this->ident, stmtSet2);
            return std::move(result);
        }
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // a(_, _) -> int[]
        {
            STMT_SET stmtSet = db->getStatements(StmtType::Assign);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(this->ident, stmtSet);
            return std::move(result);
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::EXPR):  // a(v, "_x_") -> pair<int, str>[]
        {
            std::string synonymIdent = std::dynamic_pointer_cast<Synonym>(first)->ident;
            std::vector<std::list<std::string>> vec;
            for (STMT_NUM s : stmtSet2) {  // for each statement, find entity that is modified
                ENT_SET entSet = db->getRelationship(StmtNameRelationship::Modifies, s);
                for (const std::string& ent : entSet)
                    vec.push_back({std::to_string(s), ent});
            }
            std::unique_ptr<Result> result = std::make_unique<TableResult>(this->ident, synonymIdent, vec);
            return std::move(result);
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // a(v, _) -> pair<int, str>[]
        {
            std::string synonymIdent = std::dynamic_pointer_cast<Synonym>(first)->ident;
            STMT_ENT_SET stmtEntSet = db->getAllRelationships(StmtNameRelationship::Modifies);
            std::vector<std::list<std::string>> vec;
            for (auto& p : stmtEntSet)
                vec.push_back({std::to_string(p.first), p.second});
            std::unique_ptr<Result> result = std::make_unique<TableResult>(this->ident, synonymIdent, vec);
            return std::move(result);
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
            std::unique_ptr<Result> result = std::make_unique<TableResult>(this->ident, stmtSetResult);
            return std::move(result);
        }
        case c(PQLToken::Tag::IDENT, PQLToken::Tag::WILDCARD):  // a("x", _) -> int[]
        {
            STMT_SET stmtSet1 = db->getRelationship(StmtNameRelationship::Modifies, first->str());
            std::unique_ptr<Result> result = std::make_unique<TableResult>(this->ident, stmtSet1);
            return std::move(result);
        }
        default:
            throw std::runtime_error("Pattern.cpp");
    }
}

void Pattern::validateArgs(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second) {
    std::shared_ptr<Synonym> synonym1 = std::dynamic_pointer_cast<Synonym>(first);
    if (synonym1 != NULL && synonym1->de != Synonym::DesignEntity::VARIABLE) {
        throw SemanticException();
    }
}

bool Pattern::operator==(const Clause& rhs) const {
    const Pattern* pRhs = dynamic_cast<const Pattern*>(&rhs);
    if (pRhs != nullptr) {
        return ident == pRhs->ident && equal(*pRhs);
    }
    return false;
}
