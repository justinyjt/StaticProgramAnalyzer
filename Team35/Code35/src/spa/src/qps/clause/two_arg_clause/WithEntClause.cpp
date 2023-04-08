#include "WithEntClause.h"

#include <cassert>
#include <string>

#define DUMMY_NAME "common-name"

WithEntClause::WithEntClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second) :
        TwoArgClause(std::move(first), std::move(second)) {}

STMT_ENT_SET WithEntClause::getStmtEntSet(PKBReader *db, Synonym &syn) {
    switch (syn.de) {
        case Synonym::DesignEntity::CALL :
            return db->getAllRelationships(StmtNameRelationship::CallsProcedure);
        case Synonym::DesignEntity::PRINT :
            return db->getAllRelationships(StmtNameRelationship::PrintStmtVar);
        case Synonym::DesignEntity::READ :
            return db->getAllRelationships(StmtNameRelationship::ReadStmtVar);
        default:
            throw std::runtime_error("Statement type does not exist!");
    }
}

STMT_SET WithEntClause::getStmtSet(PKBReader *db, Synonym &syn) {
    switch (syn.de) {
        case Synonym::DesignEntity::CALL :
            return db->getStatements(StmtType::Call);
        case Synonym::DesignEntity::PRINT :
            return db->getStatements(StmtType::Print);
        case Synonym::DesignEntity::READ :
            return db->getStatements(StmtType::Read);
        default:
            throw std::runtime_error("Statement type does not exist!");
    }
}

ENT_SET WithEntClause::getEntSet(PKBReader *db, Synonym &syn) {
    switch (syn.de) {
        case Synonym::DesignEntity::PROCEDURE :
            return db->getEntities(Entity::Procedure);
        case Synonym::DesignEntity::VARIABLE :
            return db->getEntities(Entity::Variable);
        default:
            throw std::runtime_error("Entity type does not exist!");
    }
}

bool WithEntClause::isStmtSyn(Synonym &syn) {
    return syn.de == Synonym::DesignEntity::CALL || syn.de == Synonym::DesignEntity::READ
            || syn.de == Synonym::DesignEntity::PRINT;
}

std::unique_ptr<Result> WithEntClause::handleSameSynCase(PKBReader *db, Synonym &syn) {
    std::unique_ptr<Result> res;
    if (isStmtSyn(syn)) {
        STMT_SET stmtSet = getStmtSet(db, syn);
        res = std::make_unique<TableResult>(syn.str(), stmtSet);
    } else {
        ENT_SET entSet = getEntSet(db, syn);
        res = std::make_unique<TableResult>(syn.str(), entSet);
    }
    return std::move(res);
}

std::unique_ptr<Result> WithEntClause::handleStmtStmt(PKBReader *db, Synonym &first, Synonym &second) {
    STMT_ENT_SET syn1StmtEntSet = getStmtEntSet(db, first);
    STMT_ENT_SET syn2StmtEntSet = getStmtEntSet(db, second);
    std::unique_ptr<Result> syn1Res = std::make_unique<TableResult>(first.str(), DUMMY_NAME, syn1StmtEntSet);
    std::unique_ptr<Result> syn2Res = std::make_unique<TableResult>(second.str(), DUMMY_NAME, syn2StmtEntSet);
    return std::move(syn1Res->join(*syn2Res));
}

std::unique_ptr<Result> WithEntClause::handleStmtEnt(PKBReader *db, Synonym &first, Synonym &second) {
    STMT_ENT_SET syn1StmtEntSet = getStmtEntSet(db, first);
    ENT_SET syn2EntSet = getEntSet(db, second);
    std::unique_ptr<Result> syn1Res = std::make_unique<TableResult>(first.str(), second.str(), syn1StmtEntSet);
    std::unique_ptr<Result> syn2Res = std::make_unique<TableResult>(second.str(), syn2EntSet);
    return std::move(syn1Res->join(*syn2Res));
}

std::unique_ptr<Result> WithEntClause::handleEntEnt(PKBReader *db, Synonym &first, Synonym &second) {
    ENT_SET syn1EntSet = getEntSet(db, first);
    ENT_SET syn2EntSet = getEntSet(db, second);
    ENT_ENT_SET result;

    for (auto const &ent : syn1EntSet) {
        for (auto const &ent2 : syn2EntSet) {
            if (ent == ent2) {
                result.emplace(ent, ent);
            }
        }
    }

    return std::make_unique<TableResult>(first.str(), second.str(), result);
}

std::unique_ptr<Result> WithEntClause::handleSynSyn(PKBReader *db, Synonym &first, Synonym &second) {
    if (first.str() == second.str()) {
        return handleSameSynCase(db, first);
    }
    bool isSyn1Stmt = isStmtSyn(first);
    bool isSyn2Stmt = isStmtSyn(second);

    if (isSyn1Stmt && isSyn2Stmt) {
        return handleStmtStmt(db, first, second);
    } else if (isSyn1Stmt) {
        return handleStmtEnt(db, first, second);
    } else if (isSyn2Stmt) {
        return handleStmtEnt(db, second, first);
    } else {
        return handleEntEnt(db, first, second);
    }
}

std::unique_ptr<Result> WithEntClause::handleStmtIdent(PKBReader *db, Synonym &stmt, Ident &ident) {
    STMT_ENT_SET syn1StmtEntSet = getStmtEntSet(db, stmt);
    STMT_SET resultSet;
    for (auto const &stmtEntPair : syn1StmtEntSet) {
        STMT_NUM synStmt = stmtEntPair.first;
        ENT_NAME synEnt = stmtEntPair.second;
        if (ident.str() == synEnt) {
            resultSet.emplace(synStmt);
        }
    }
    return std::make_unique<TableResult>(stmt.str(), resultSet);
}

std::unique_ptr<Result> WithEntClause::handleEntIdent(PKBReader *db, Synonym &ent, Ident &ident) {
    ENT_SET synEntSet = getEntSet(db, ent);
    ENT_SET resultSet;
    for (auto const &synEnt : synEntSet) {
        if (synEnt == ident.str()) {
            resultSet.emplace(ident.str());
        }
    }
    return std::make_unique<TableResult>(ent.str(), resultSet);
}

std::unique_ptr<Result> WithEntClause::handleSynIdent(PKBReader *db, Synonym &first, Ident &second) {
    if (first.de == Synonym::DesignEntity::CALL || first.de == Synonym::DesignEntity::READ
            || first.de == Synonym::DesignEntity::PRINT) {
        return handleStmtIdent(db, first, second);
    } else {
        return handleEntIdent(db, first, second);
    }
}

std::unique_ptr<Result> WithEntClause::evaluate(PKBReader *db) {
    /* <SYNONYM | IDENT>, <SYNONYM | IDENT> */

    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM) :  // with cl.procname = p.procname -> syn_type[]
            return handleSynSyn(db, dynamic_cast<Synonym &>(*first_), dynamic_cast<Synonym &>(*second_));
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT) :  // with syn.x = "x" -> syn_type[]
            return handleSynIdent(db, dynamic_cast<Synonym &>(*first_), dynamic_cast<Ident &>(*second_));
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::SYNONYM) :  // with syn.x = "x" -> syn_type[]
            return handleSynIdent(db, dynamic_cast<Synonym &>(*second_), dynamic_cast<Ident &>(*first_));
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::IDENT) :  // Uses/Modifies(1, "x") -> bool
            return std::make_unique<BoolResult>(
                    (dynamic_cast<Ident &>(*first_)).s == (dynamic_cast<Ident &>(*second_)).s);
        default:
            throw std::runtime_error("");
    }
}

void WithEntClause::validateArgs() {}

bool WithEntClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const WithEntClause *>(&rhs);
    return pRhs != nullptr && TwoArgClause::equal(*pRhs);
}
