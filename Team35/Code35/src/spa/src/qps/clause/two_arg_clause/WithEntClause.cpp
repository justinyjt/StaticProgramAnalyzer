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
            assert(false);
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
            assert(false);
    }
}

ENT_SET WithEntClause::getEntSet(PKBReader *db, Synonym &syn) {
    switch (syn.de) {
        case Synonym::DesignEntity::PROCEDURE :
            return db->getEntities(Entity::Procedure);
        case Synonym::DesignEntity::VARIABLE :
            return db->getEntities(Entity::Variable);
        default:
            assert(false);
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

std::unique_ptr<Result> WithEntClause::handleStmtStmt(PKBReader *db, Synonym &syn1, Synonym &syn2) {
    STMT_ENT_SET syn1StmtEntSet = getStmtEntSet(db, syn1);
    STMT_ENT_SET syn2StmtEntSet = getStmtEntSet(db, syn2);
    std::unique_ptr<Result> syn1Res = std::make_unique<TableResult>(syn1.str(), DUMMY_NAME, syn1StmtEntSet);
    std::unique_ptr<Result> syn2Res = std::make_unique<TableResult>(syn2.str(), DUMMY_NAME, syn2StmtEntSet);
    return std::move(syn1Res->join(*syn2Res));
}

std::unique_ptr<Result> WithEntClause::handleStmtEnt(PKBReader *db, Synonym &stmtSyn, Synonym &entSyn) {
    STMT_ENT_SET syn1StmtEntSet = getStmtEntSet(db, stmtSyn);
    ENT_SET syn2EntSet = getEntSet(db, entSyn);
    std::unique_ptr<Result> syn1Res = std::make_unique<TableResult>(stmtSyn.str(), entSyn.str(), syn1StmtEntSet);
    std::unique_ptr<Result> syn2Res = std::make_unique<TableResult>(entSyn.str(), syn2EntSet);
    return std::move(syn1Res->join(*syn2Res));
}

std::unique_ptr<Result> WithEntClause::handleEntEnt(PKBReader *db, Synonym &syn1, Synonym &syn2) {
    ENT_SET syn1EntSet = getEntSet(db, syn1);
    ENT_SET syn2EntSet = getEntSet(db, syn2);
    std::vector<ENT_NAME> resultVec;

    for (auto const &ent : syn1EntSet) {
        for (auto const &ent2 : syn2EntSet) {
            if (ent == ent2) {
                resultVec.emplace_back(ent);
            }
        }
    }

    std::unique_ptr<Result> res = std::make_unique<TableResult>(syn1.str(), syn2.str(), resultVec);
    return std::move(res);
}

std::unique_ptr<Result> WithEntClause::handleSynSyn(PKBReader *db, Synonym &syn1, Synonym &syn2) {
    if (syn1.str() == syn2.str()) {
        return handleSameSynCase(db, syn1);
    }
    bool isSyn1Stmt = isStmtSyn(syn1);
    bool isSyn2Stmt = isStmtSyn(syn2);

    if (isSyn1Stmt && isSyn2Stmt) {
        return handleStmtStmt(db, syn1, syn2);
    } else if (isSyn1Stmt) {
        return handleStmtEnt(db, syn1, syn2);
    } else if (isSyn2Stmt) {
        return handleStmtEnt(db, syn2, syn1);
    } else {
        return handleEntEnt(db, syn1, syn2);
    }
}

std::unique_ptr<Result> WithEntClause::handleSynEnt(PKBReader *db, Synonym &syn, ENT_NAME &ent) {
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
            Synonym syn1 = dynamic_cast<Synonym &>(*first_);
            Synonym syn2 = dynamic_cast<Synonym &>(*second_);
            return handleSynSyn(db, syn1, syn2);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT) :  // with syn.x = "x" -> syn_type[]
        {
            Synonym syn = dynamic_cast<Synonym &>(*first_);
            ENT_NAME ent = (dynamic_cast<Ident &>(*second_)).s;
            return handleSynEnt(db, syn, ent);
        }
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::SYNONYM) :  // with syn.x = "x" -> syn_type[]
        {
            Synonym syn = dynamic_cast<Synonym &>(*second_);
            ENT_NAME ent = (dynamic_cast<Ident &>(*first_)).s;
            return handleSynEnt(db, syn, ent);
        }
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::IDENT) :  // Uses/Modifies(1, "x") -> bool
        {
            ENT_NAME ent1 = (dynamic_cast<Ident &>(*first_)).s;
            ENT_NAME ent2 = (dynamic_cast<Ident &>(*second_)).s;
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
