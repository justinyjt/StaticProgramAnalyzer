#include "StmtEntClause.h"

#include <string>

StmtEntClause::StmtEntClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second,
                             StmtNameRelationship rs) : TwoArgClause(std::move(first), std::move(second)), rs(rs) {}

std::unique_ptr<Result> StmtEntClause::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */

    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Uses/Modifies(1, var) -> string[]
        {
            std::string stmtNum = dynamic_cast<PQLNumber &>(*first).n;
            ENT_SET set = db->getRelationship(rs, std::stoi(stmtNum));
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), set);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Uses/Modifies(1, _) -> bool
        {
            std::string stmtNum = dynamic_cast<PQLNumber &>(*first).n;
            ENT_SET set = db->getRelationship(rs, std::stoi(stmtNum));
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!set.empty());
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::IDENT):  // Uses/Modifies(1, "x") -> bool
        {
            std::string stmtNum = dynamic_cast<PQLNumber &>(*first).n;
            bool b = db->isRelationshipExists(rs, std::stoi(stmtNum), second->str());
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(b);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Uses/Mod(stmt, var) -> <int, string>[]
        {
            STMT_ENT_SET stmtEntSet = db->getAllRelationships(rs);
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<Synonym &>(*first).de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(first->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), second->str(), stmtEntSet);
            return std::move(result->join(*intermediateResult));
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Uses/Modifies(stmt, _) -> int[]
        {
            STMT_SET stmtSet = db->getStmtByRelationship(rs);
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<Synonym &>(*first).de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(first->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), stmtSet);
            return std::move(result->join(*intermediateResult));
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT):  // Uses/Modifies(stmt, "x") -> int[]
        {
            STMT_SET stmtSet = db->getRelationship(rs, second->str());
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<Synonym &>(*first).de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(first->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), stmtSet);
            return std::move(result->join(*intermediateResult));
        }
        default:
            throw std::runtime_error("");
    }
}

bool StmtEntClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const StmtEntClause *>(&rhs);
    return pRhs != nullptr && rs == pRhs->rs && TwoArgClause::equal(*pRhs);
}


UsesS::UsesS(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second) :
        StmtEntClause(std::move(first), std::move(second), StmtNameRelationship::Uses) {
    validateArgs();
}

void UsesS::validateArgs() {
    Synonym *synonym1 = dynamic_cast<Synonym *>(first.get());
    Synonym *synonym2 = dynamic_cast<Synonym *>(second.get());
    if (synonym1 != nullptr && (synonym1->de == Synonym::DesignEntity::READ) ||
        first->tag == PQLToken::Tag::WILDCARD ||
        (synonym2 != nullptr && synonym2->de != Synonym::DesignEntity::VARIABLE)) {
        throw SemanticException();
    }
}


ModifiesS::ModifiesS(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second) :
        StmtEntClause(std::move(first), std::move(second), StmtNameRelationship::Modifies) {
    validateArgs();
}

void ModifiesS::validateArgs() {
    Synonym *synonym1 = dynamic_cast<Synonym *>(first.get());
    Synonym *synonym2 = dynamic_cast<Synonym *>(second.get());
    if (synonym1 != nullptr && (synonym1->de == Synonym::DesignEntity::PRINT) ||
        first->tag == PQLToken::Tag::WILDCARD ||
        (synonym2 != nullptr && synonym2->de != Synonym::DesignEntity::VARIABLE)) {
        throw SemanticException();
    }
}
