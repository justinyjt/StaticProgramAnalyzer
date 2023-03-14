#include "WithEntClause.h"

#include <cassert>

WithEntClause::WithEntClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second) : TwoArgClause(std::move(first), std::move(second)) {};

ENT_SET WithEntClause::getEntValuesFromSyn(Synonym syn, PKBReader* db) {
    switch (syn.de) {
        case Synonym::DesignEntity::PROCEDURE: {
            return db->getEntities(Entity::Procedure);
        }
        case Synonym::DesignEntity::CALL: {
            return db->getKeys(NameNameRelationship::Calls);
        }
        case Synonym::DesignEntity::VARIABLE: {
            return db->getEntities(Entity::Variable);
        }
        case Synonym::DesignEntity::READ:
        {
            STMT_SET readStmts = db->getStatements(StmtType::Read);
            ENT_SET readValues;
            for (auto const& readStmt : readStmts) {
                ENT_SET modifiedVars = db->getRelationship(StmtNameRelationship::Modifies, readStmt);
                for (auto const& modifiedVar : modifiedVars) {
                    readValues.emplace(modifiedVar);
                }
            }
            return readValues;
        }
        case Synonym::DesignEntity::PRINT:
        {
            STMT_SET printStmts = db->getStatements(StmtType::Print);
            ENT_SET printValues;
            for (auto const& printStmt : printStmts) {
                ENT_SET usedVars = db->getRelationship(StmtNameRelationship::Uses, printStmt);
                for (auto const& usedVar : usedVars) {
                    printValues.emplace(usedVar);
                }
            }
            return printValues;
        }
        default:
            assert(false);
    }
}

std::unique_ptr<Result> WithEntClause::evaluate(PKBReader* db) {
    /* <SYNONYM | IDENT>, <SYNONYM | IDENT> */

    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // with syn.x = syn.y -> syn_type[]
        {
            Synonym syn1 = dynamic_cast<Synonym&>(*first);
            Synonym syn2 = dynamic_cast<Synonym&>(*second);
            ENT_SET syn1Vals = getEntValuesFromSyn(syn1, db);
            ENT_SET syn2Vals = getEntValuesFromSyn(syn2, db);
            ENT_ENT_SET resultSet;
            for (auto const& syn1Val : syn1Vals) {
                for (auto const& syn2Val : syn2Vals) {
                    if (syn1Val == syn2Val) {
                        resultSet.emplace(ENT_ENT(syn1Val, syn2Val));
                    }
                }
            }
            std::unique_ptr<Result> res = std::make_unique<TableResult>(first->str(), second->str(), resultSet);
            return std::move(res);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT):  // with syn.x = "x" -> syn_type[]
        {
            Synonym syn1 = dynamic_cast<Synonym&>(*first);
            ENT_NAME ent = (dynamic_cast<Ident &>(*second)).s;
            ENT_SET syn1Vals = getEntValuesFromSyn(syn1, db);
            ENT_SET resultSet;
            for (auto const& syn1Val : syn1Vals) {
                if (syn1Val == ent) {
                    resultSet.emplace(syn1Val);
                }
            }
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), resultSet);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::SYNONYM):  // with syn.x = "x" -> syn_type[]
        {
            Synonym syn2 = dynamic_cast<Synonym&>(*second);
            ENT_NAME ent = (dynamic_cast<Ident &>(*first)).s;
            ENT_SET syn2Vals = getEntValuesFromSyn(syn2, db);
            ENT_SET resultSet;
            for (auto const& syn2Val : syn2Vals) {
                if (syn2Val == ent) {
                    resultSet.emplace(syn2Val);
                }
            }
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), resultSet);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::IDENT):  // Uses/Modifies(1, "x") -> bool
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
    const auto* pRhs = dynamic_cast<const WithEntClause*>(&rhs);
    return pRhs != nullptr && TwoArgClause::equal(*pRhs);
}
