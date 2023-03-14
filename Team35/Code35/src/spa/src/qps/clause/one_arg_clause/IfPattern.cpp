#include <list>

#include "IfPattern.h"

IfPattern::IfPattern(std::unique_ptr<PQLToken> first,
                           std::string ident) : OneArgClause(std::move(first)), ident(ident) {
    validateArgs();
}

std::unique_ptr<Result> IfPattern::evaluate(PKBReader *db) {
    STMT_SET ifStatements = db->getStatements(StmtType::If);
    switch (first->tag) {
        case PQLToken::Tag::WILDCARD: {  // ifs(_, _, _) -> int[]
            return std::make_unique<TableResult>(ifStatements);
        }
        case PQLToken::Tag::IDENT: {  // ifs("x", _, _) -> int[]
            STMT_SET s = db->getRelationship(StmtNameRelationship::Uses, 
                                dynamic_cast<Ident&>(*first).s);
            STMT_SET result;
            for (int stmt : ifStatements) {
                if (s.find(stmt) != s.end()) {
                    result.insert(stmt);
                }
            }
            return std::make_unique<TableResult>(result);
        }
        case PQLToken::Tag::SYNONYM: {  // ifs(x, _, _) -> <int, str>[]
            STMT_ENT_SET se = db->getAllRelationships(StmtNameRelationship::Uses);
            STMT_ENT_SET result;
            for (auto& p : se) {
                if (ifStatements.find(p.first) != ifStatements.end()) {
                    result.insert(p);
                }
            }
            return std::make_unique<TableResult>(result);
        }
        default: throw std::runtime_error("IfPattern");
    }
}

void IfPattern::validateArgs() {
    Synonym* synonym = dynamic_cast<Synonym*>(first.get());
    if (synonym != nullptr && !(synonym->de == Synonym::DesignEntity::PROCEDURE ||
            synonym->de == Synonym::DesignEntity::VARIABLE ||
            synonym->de == Synonym::DesignEntity::CONSTANT)) {
        throw SemanticException();
    }
}

bool IfPattern::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const IfPattern *>(&rhs);
    return pRhs != nullptr && equal(*pRhs) && ident == pRhs->ident;
}
