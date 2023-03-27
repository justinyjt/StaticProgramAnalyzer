#include "OneArgClause.h"

#include <utility>

OneArgClause::OneArgClause(std::unique_ptr<PQLToken> first, StmtNameRelationship rs, std::string ident) :
    first(std::move(first)), rs(rs), ident(std::move(ident)) {}

std::unique_ptr<Result> OneArgClause::evaluate(PKBReader *db) {
    switch (first->tag) {
        case PQLToken::Tag::WILDCARD: {  // ifs(_, _, _) -> int[]
            STMT_SET stmts = db->getStmtByRelationship(rs);
            return std::make_unique<TableResult>(ident, stmts);
        }
        case PQLToken::Tag::IDENT: {  // ifs("x", _, _) -> int[]
            auto &id = dynamic_cast<Ident &>(*first);
            STMT_SET stmts = db->getRelationship(rs, id.s);
            return std::make_unique<TableResult>(ident, stmts);
        }
        case PQLToken::Tag::SYNONYM: {  // ifs(x, _, _) -> <str, str>[]
            auto &syn = dynamic_cast<Synonym &>(*first);
            STMT_ENT_SET stmt_ent_set = db->getAllRelationships(rs);
            return std::make_unique<TableResult>(ident, syn.ident, stmt_ent_set);
        }
        default:
            throw std::runtime_error("IfPattern");
    }
}

bool OneArgClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const OneArgClause *>(&rhs);
    return pRhs != nullptr && rs == pRhs->rs && ident == pRhs->ident && *first == *(pRhs->first);
}

void OneArgClause::validateArgs() {
    auto *synonym = dynamic_cast<Synonym *>(first.get());
    if (synonym != nullptr && !(synonym->de == Synonym::DesignEntity::PROCEDURE ||
        synonym->de == Synonym::DesignEntity::VARIABLE ||
        synonym->de == Synonym::DesignEntity::CONSTANT)) {
        throw SemanticException();
    }
}

IfPattern::IfPattern(std::unique_ptr<PQLToken> first, std::string ident) :
    OneArgClause(std::move(first), StmtNameRelationship::IfCondVarUses, std::move(ident)) {
    validateArgs();
}

WhilePattern::WhilePattern(std::unique_ptr<PQLToken> first, std::string ident) :
    OneArgClause(std::move(first), StmtNameRelationship::WhileCondVarUses, std::move(ident)) {
    validateArgs();
}
