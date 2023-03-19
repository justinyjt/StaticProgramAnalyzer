#include "OneArgClause.h"

#include <cassert>

OneArgClause::OneArgClause(std::unique_ptr<PQLToken> first, StmtNameRelationship rs, std::string ident) :
                    first(std::move(first)), rs(rs), ident(ident) {}

std::unique_ptr<Result> OneArgClause::evaluate(PKBReader *db) {
    switch (first->tag) {
        case PQLToken::Tag::WILDCARD: {  // ifs(_, _, _) -> int[]
            STMT_SET stmts = db->getStatements(getStmtType());
            return std::make_unique<TableResult>(ident, stmts);
        }
        case PQLToken::Tag::IDENT: {  // ifs("x", _, _) -> int[]
            Ident& id = dynamic_cast<Ident&>(*first);
            STMT_SET stmts = db->getRelationship(rs, id.s);
            return std::make_unique<TableResult>(ident, stmts);
        }
        case PQLToken::Tag::SYNONYM: {  // ifs(x, _, _) -> <str, str>[]
            Synonym& syn = dynamic_cast<Synonym&>(*first);
            STMT_ENT_SET stmtents = db->getAllRelationships(rs);
            return std::make_unique<TableResult>(ident, syn.ident, stmtents);
        }
        default: throw std::runtime_error("IfPattern");
    }
}

StmtType OneArgClause::getStmtType() const {
    switch (rs) {
        case StmtNameRelationship::IfCondVarUses: return StmtType::If;
        case StmtNameRelationship::WhileCondVarUses: return StmtType::While;
        default: throw std::runtime_error("OneArgClause");
    }
}

bool OneArgClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const OneArgClause *>(&rhs);
    return pRhs != nullptr && rs == pRhs->rs && ident == pRhs->ident && *first == *(pRhs->first);
}

void OneArgClause::validateArgs() {
    Synonym* synonym = dynamic_cast<Synonym*>(first.get());
    if (synonym != nullptr && !(synonym->de == Synonym::DesignEntity::PROCEDURE ||
            synonym->de == Synonym::DesignEntity::VARIABLE ||
            synonym->de == Synonym::DesignEntity::CONSTANT)) {
        throw SemanticException();
    }
}


IfPattern::IfPattern(std::unique_ptr<PQLToken> first, std::string ident) :
                OneArgClause(std::move(first), StmtNameRelationship::IfCondVarUses, ident) {
    validateArgs();
}


WhilePattern::WhilePattern(std::unique_ptr<PQLToken> first, std::string ident) :
                OneArgClause(std::move(first), StmtNameRelationship::WhileCondVarUses, ident) {
    validateArgs();
}
