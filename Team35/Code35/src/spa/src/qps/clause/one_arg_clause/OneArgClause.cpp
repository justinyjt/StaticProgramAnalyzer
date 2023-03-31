#include "OneArgClause.h"

#include <utility>

OneArgClause::OneArgClause(std::unique_ptr<PQLToken> first, StmtNameRelationship rs, std::string ident) :
    first_(std::move(first)), rs_(rs), ident_(std::move(ident)) {
    this->addHeader(ident_);
    if (first_->tag == PQLToken::Tag::SYNONYM)
        this->addHeader(dynamic_cast<Synonym &>(*first_).ident);
}

std::unique_ptr<Result> OneArgClause::evaluate(PKBReader *db) {
    switch (first_->tag) {
        case PQLToken::Tag::WILDCARD: {  // ifs(_, _, _) -> int[]
            STMT_SET stmts = db->getStmtByRelationship(rs_);
            return std::make_unique<TableResult>(ident_, stmts);
        }
        case PQLToken::Tag::IDENT: {  // ifs("x", _, _) -> int[]
            auto &id = dynamic_cast<Ident &>(*first_);
            STMT_SET stmts = db->getRelationship(rs_, id.s);
            return std::make_unique<TableResult>(ident_, stmts);
        }
        case PQLToken::Tag::SYNONYM: {  // ifs(x, _, _) -> <str, str>[]
            auto &syn = dynamic_cast<Synonym &>(*first_);
            STMT_ENT_SET stmt_ent_set = db->getAllRelationships(rs_);
            return std::make_unique<TableResult>(ident_, syn.ident, stmt_ent_set);
        }
        default:
            throw std::runtime_error("IfPattern");
    }
}

bool OneArgClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const OneArgClause *>(&rhs);
    return pRhs != nullptr && rs_ == pRhs->rs_ && ident_ == pRhs->ident_ && *first_ == *(pRhs->first_);
}

void OneArgClause::validateArgs() {
    auto *synonym = dynamic_cast<Synonym *>(first_.get());
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
