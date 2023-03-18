#include "OneArgClause.h"

#include <cassert>

OneArgClause::OneArgClause(std::unique_ptr<PQLToken> first, StmtNameRelationship rs, std::string ident) :
                    first(std::move(first)), rs(rs), ident(ident) {}

std::unique_ptr<Result> OneArgClause::evaluate(PKBReader *db) {
    STMT_SET stmts = db->getStatements(StmtType::If);
    switch (first->tag) {
        case PQLToken::Tag::WILDCARD: {  // ifs(_, _, _) -> bool
            return std::make_unique<BoolResult>(!stmts.empty());
        }
        case PQLToken::Tag::IDENT: {  // ifs("x", _, _) -> bool
            STMT_SET s = db->getRelationship(StmtNameRelationship::IfCondVarUses,
                                dynamic_cast<Ident&>(*first).s);
            STMT_SET result;
            for (int stmt : stmts) {
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
                if (stmts.find(p.first) != stmts.end()) {
                    result.insert(p.second);
                }
            }
            return std::make_unique<TableResult>(dynamic_cast<Synonym&>(*first).ident, result);
        }
        default: throw std::runtime_error("IfPattern");
    }
}

bool OneArgClause::equal(const OneArgClause &rhs) const {
    return *first == *(rhs.first) && ident == rhs.ident;
}

// If Pattern

IfPattern::IfPattern(std::unique_ptr<PQLToken> first, std::string ident) :
                OneArgClause(std::move(first), StmtNameRelationship::IfCondVarUses, ident) {
    validateArgs();
}

bool IfPattern::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const IfPattern *>(&rhs);
    return pRhs != nullptr && equal(*pRhs);
}

void IfPattern::validateArgs() {
    Synonym* synonym = dynamic_cast<Synonym*>(first.get());
    if (synonym != nullptr && !(synonym->de == Synonym::DesignEntity::PROCEDURE ||
            synonym->de == Synonym::DesignEntity::VARIABLE ||
            synonym->de == Synonym::DesignEntity::CONSTANT)) {
        throw SemanticException();
    }
}

// While Pattern

WhilePattern::WhilePattern(std::unique_ptr<PQLToken> first, std::string ident) :
                OneArgClause(std::move(first), StmtNameRelationship::WhileCondVarUses, ident) {
    validateArgs();
}

bool WhilePattern::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const WhilePattern *>(&rhs);
    return pRhs != nullptr && equal(*pRhs);
}

void WhilePattern::validateArgs() {
    Synonym* synonym = dynamic_cast<Synonym*>(first.get());
    if (synonym != nullptr && !(synonym->de == Synonym::DesignEntity::PROCEDURE ||
            synonym->de == Synonym::DesignEntity::VARIABLE ||
            synonym->de == Synonym::DesignEntity::CONSTANT)) {
        throw SemanticException();
    }
}
