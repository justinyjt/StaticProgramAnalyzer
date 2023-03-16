#include <list>

#include "IfPattern.h"

IfPattern::IfPattern(std::unique_ptr<PQLToken> first,
                           std::string ident) : OneArgClause(std::move(first)), ident(ident) {
    validateArgs();
}

std::unique_ptr<Result> IfPattern::evaluate(PKBReader *db) {
    STMT_SET ifStatements = db->getStatements(StmtType::If);
    switch (first->tag) {
        case PQLToken::Tag::WILDCARD: {  // ifs(_, _, _) -> bool
            return std::make_unique<BoolResult>(!ifStatements.empty());
        }
        case PQLToken::Tag::IDENT: {  // ifs("x", _, _) -> bool
            STMT_SET s = db->getRelationship(StmtNameRelationship::IfCondVarUses, 
                                dynamic_cast<Ident&>(*first).s);
            STMT_SET result;
            for (int stmt : ifStatements) {
                if (s.find(stmt) != s.end()) {
                    result.insert(stmt);
                }
            }
            return std::make_unique<BoolResult>(!result.empty());
        }
        case PQLToken::Tag::SYNONYM: {  // ifs(x, _, _) -> str[]
            STMT_ENT_SET se = db->getAllRelationships(StmtNameRelationship::IfCondVarUses);
            ENT_SET result;
            for (auto& p : se) {
                if (ifStatements.find(p.first) != ifStatements.end()) {
                    result.insert(p.second);
                }
            }
            return std::make_unique<TableResult>(dynamic_cast<Synonym&>(*first).ident, result);
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
