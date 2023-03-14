#include "WhilePattern.h"

#include <list>

WhilePattern::WhilePattern(std::unique_ptr<PQLToken> first,
                             std::string ident) : OneArgClause(std::move(first)), ident(ident) {
    validateArgs();
}

std::unique_ptr<Result> WhilePattern::evaluate(PKBReader *db) {
    STMT_SET whileStatements = db->getStatements(StmtType::While);
    switch (first->tag) {
        case PQLToken::Tag::WILDCARD: {  // w(_, _) -> int[]
            return std::make_unique<TableResult>(whileStatements);
        }
        case PQLToken::Tag::IDENT: {  // w("x", _) -> int[]
            STMT_SET s = db->getRelationship(StmtNameRelationship::Uses, 
                                dynamic_cast<Ident&>(*first).s);
            STMT_SET result;
            for (int stmt : whileStatements) {
                if (s.find(stmt) != s.end()) {
                    result.insert(stmt);
                }
            }
            return std::make_unique<TableResult>(result);
        }
        case PQLToken::Tag::SYNONYM: {  // w(x, _) -> <int, str>[]
            STMT_ENT_SET se = db->getAllRelationships(StmtNameRelationship::Uses);
            STMT_ENT_SET result;
            for (auto& p : se) {
                if (whileStatements.find(p.first) != whileStatements.end()) {
                    result.insert(p);
                }
            }
            return std::make_unique<TableResult>(result);
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
