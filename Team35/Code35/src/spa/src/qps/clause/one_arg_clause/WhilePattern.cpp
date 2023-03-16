#include "WhilePattern.h"

#include <list>

WhilePattern::WhilePattern(std::unique_ptr<PQLToken> first,
                             std::string ident) : OneArgClause(std::move(first)), ident(ident) {
    validateArgs();
}

std::unique_ptr<Result> WhilePattern::evaluate(PKBReader *db) {
    STMT_SET whileStatements = db->getStatements(StmtType::While);
    switch (first->tag) {
        case PQLToken::Tag::WILDCARD: {  // w(_, _) -> bool
            return std::make_unique<BoolResult>(!whileStatements.empty());
        }
        case PQLToken::Tag::IDENT: {  // w("x", _) -> bool
            STMT_SET s = db->getRelationship(StmtNameRelationship::WhileCondVarUses, 
                                dynamic_cast<Ident&>(*first).s);
            STMT_SET result;
            for (int stmt : whileStatements) {
                if (s.find(stmt) != s.end()) {
                    result.insert(stmt);
                }
            }
            return std::make_unique<BoolResult>(!result.empty());
        }
        case PQLToken::Tag::SYNONYM: {  // w(x, _) -> str[]
            STMT_ENT_SET se = db->getAllRelationships(StmtNameRelationship::WhileCondVarUses);
            ENT_SET result;
            for (auto& p : se) {
                if (whileStatements.find(p.first) != whileStatements.end()) {
                    result.insert(p.second);
                }
            }
            return std::make_unique<TableResult>(dynamic_cast<Synonym&>(*first).ident, result);
        }
        default: throw std::runtime_error("WhilePattern");
    }
}

void WhilePattern::validateArgs() {
    Synonym* synonym = dynamic_cast<Synonym*>(first.get());
    if (synonym != nullptr && !(synonym->de == Synonym::DesignEntity::PROCEDURE ||
            synonym->de == Synonym::DesignEntity::VARIABLE ||
            synonym->de == Synonym::DesignEntity::CONSTANT)) {
        throw SemanticException();
    }
}

bool WhilePattern::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const WhilePattern *>(&rhs);
    return pRhs != nullptr && equal(*pRhs) && ident == pRhs->ident;
}
