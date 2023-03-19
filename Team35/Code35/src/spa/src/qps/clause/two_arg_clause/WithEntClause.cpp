#include "WithEntClause.h"

#include <cassert>
#include <string>

#define DUMMY_NAME "common-name"

WithEntClause::WithEntClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second) :
    TwoArgClause(std::move(first), std::move(second)) {}

STMT_ENT_SET WithEntClause::getStmtEntSet(PKBReader *db, Synonym syn) {
    switch (syn.de) {
        case Synonym::DesignEntity::CALL :
            return db->getAllRelationships(StmtNameRelationship::CallsProcedure);
        case Synonym::DesignEntity::PRINT :
            return db->getAllRelationships(StmtNameRelationship::PrintStmtVar);
        case Synonym::DesignEntity::READ :
            return db->getAllRelationships(StmtNameRelationship::ReadStmtVar);
        default:
            assert(false);
    }
}

ENT_SET WithEntClause::getEntSet(PKBReader *db, Synonym syn) {
    switch (syn.de) {
        case Synonym::DesignEntity::PROCEDURE :
            return db->getEntities(Entity::Procedure);
        case Synonym::DesignEntity::VARIABLE :
            return db->getEntities(Entity::Variable);
        default:
            assert(false);
    }
}

std::unique_ptr<Result> WithEntClause::handleSynSyn(PKBReader *db, Synonym syn1, Synonym syn2) {
    bool isSyn1Stmt = syn1.de == Synonym::DesignEntity::CALL || syn1.de == Synonym::DesignEntity::READ
        || syn1.de == Synonym::DesignEntity::PRINT;
    bool isSyn2Stmt = syn2.de == Synonym::DesignEntity::CALL || syn2.de == Synonym::DesignEntity::READ
        || syn2.de == Synonym::DesignEntity::PRINT;
    if (isSyn1Stmt && isSyn2Stmt) {
        STMT_ENT_SET syn1StmtEntSet = getStmtEntSet(db, syn1);
        STMT_ENT_SET syn2StmtEntSet = getStmtEntSet(db, syn2);
        std::unique_ptr<Result> syn1Res = std::make_unique<TableResult>(syn1.str(), DUMMY_NAME, syn1StmtEntSet);
        std::unique_ptr<Result> syn2Res = std::make_unique<TableResult>(syn2.str(), DUMMY_NAME, syn2StmtEntSet);
        return std::move(Result::join(*syn1Res, *syn2Res));
    } else if (isSyn1Stmt) {
        STMT_ENT_SET syn1StmtEntSet = getStmtEntSet(db, syn1);
        ENT_SET syn2EntSet = getEntSet(db, syn2);
        std::unique_ptr<Result> syn1Res = std::make_unique<TableResult>(syn1.str(), syn2.str(), syn1StmtEntSet);
        std::unique_ptr<Result> syn2Res = std::make_unique<TableResult>(syn2.str(), syn2EntSet);
        return std::move(Result::join(*syn1Res, *syn2Res));
    } else if (isSyn2Stmt) {
        STMT_ENT_SET syn2StmtEntSet = getStmtEntSet(db, syn2);
        ENT_SET syn1EntSet = getEntSet(db, syn1);
        std::unique_ptr<Result> syn1Res = std::make_unique<TableResult>(syn2.str(), syn1.str(), syn2StmtEntSet);
        std::unique_ptr<Result> syn2Res = std::make_unique<TableResult>(syn1.str(), syn1EntSet);
        return std::move(Result::join(*syn1Res, *syn2Res));
    } else {
        ENT_SET syn1EntSet = getEntSet(db, syn1);
        ENT_SET syn2EntSet = getEntSet(db, syn2);
        ENT_SET resultSet;
        for (auto const &ent : syn1EntSet) {
            if (syn2EntSet.find(ent) != syn2EntSet.end()) {
                resultSet.emplace(ent);
            }
        }
        std::unique_ptr<Result> syn1Res = std::make_unique<TableResult>(syn1.str(), resultSet);
        std::unique_ptr<Result> syn2Res = std::make_unique<TableResult>(syn2.str(), resultSet);
        return std::move(Result::join(*syn1Res, *syn2Res));
    }
}

std::unique_ptr<Result> WithEntClause::handleSynEnt(PKBReader *db, Synonym syn, ENT_NAME ent) {
    if (syn.de == Synonym::DesignEntity::CALL || syn.de == Synonym::DesignEntity::READ
        || syn.de == Synonym::DesignEntity::PRINT) {
        STMT_ENT_SET syn1StmtEntSet = getStmtEntSet(db, syn);
        STMT_SET resultSet;
        for (auto const &stmtEntPair : syn1StmtEntSet) {
            STMT_NUM synStmt = stmtEntPair.first;
            ENT_NAME synEnt = stmtEntPair.second;
            if (ent == synEnt) {
                resultSet.emplace(synStmt);
            }
        }
        std::unique_ptr<Result> res = std::make_unique<TableResult>(syn.str(), resultSet);
        return std::move(res);
    } else {
        ENT_SET synEntSet = getEntSet(db, syn);
        ENT_SET resultSet;
        for (auto const &synEnt : synEntSet) {
            if (synEnt == ent) {
                resultSet.emplace(ent);
            }
        }
        std::unique_ptr<Result> res = std::make_unique<TableResult>(syn.str(), resultSet);
        return std::move(res);
    }
}

std::unique_ptr<Result> WithEntClause::evaluate(PKBReader *db) {
    /* <SYNONYM | IDENT>, <SYNONYM | IDENT> */

    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM) :  // with cl.procname = p.procname -> syn_type[]
        {
            Synonym syn1 = dynamic_cast<Synonym &>(*first);
            Synonym syn2 = dynamic_cast<Synonym &>(*second);
            return handleSynSyn(db, syn1, syn2);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT) :  // with syn.x = "x" -> syn_type[]
        {
            Synonym syn = dynamic_cast<Synonym &>(*first);
            ENT_NAME ent = (dynamic_cast<Ident &>(*second)).s;
            return handleSynEnt(db, syn, ent);
        }
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::SYNONYM) :  // with syn.x = "x" -> syn_type[]
        {
            Synonym syn = dynamic_cast<Synonym &>(*second);
            ENT_NAME ent = (dynamic_cast<Ident &>(*first)).s;
            return handleSynEnt(db, syn, ent);
        }
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::IDENT) :  // Uses/Modifies(1, "x") -> bool
        {
            ENT_NAME ent1 = (dynamic_cast<Ident &>(*first)).s;
            ENT_NAME ent2 = (dynamic_cast<Ident &>(*second)).s;
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(ent1 == ent2);
            return std::move(result);
        }
        default:
            throw std::runtime_error("");
    }
}

void WithEntClause::validateArgs() {}

bool WithEntClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const WithEntClause *>(&rhs);
    return pRhs != nullptr && TwoArgClause::equal(*pRhs);
}
