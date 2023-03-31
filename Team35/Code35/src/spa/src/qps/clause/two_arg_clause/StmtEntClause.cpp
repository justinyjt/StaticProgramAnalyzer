#include "StmtEntClause.h"

#include <string>

StmtEntClause::StmtEntClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second,
                             StmtNameRelationship rs) : TwoArgClause(std::move(first), std::move(second)), rs(rs) {}

std::unique_ptr<Result> StmtEntClause::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */
    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Uses/Modifies(1, var) -> string[]
        {
            std::string stmtNum = dynamic_cast<PQLNumber &>(*first_).n;
            ENT_SET set = db->getRelationship(rs, std::stoi(stmtNum));
            return std::make_unique<TableResult>(second_->str(), set);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Uses/Modifies(1, _) -> bool
        {
            std::string stmtNum = dynamic_cast<PQLNumber &>(*first_).n;
            ENT_SET set = db->getRelationship(rs, std::stoi(stmtNum));
            return std::make_unique<BoolResult>(!set.empty());
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::IDENT):  // Uses/Modifies(1, "x") -> bool
        {
            std::string stmtNum = dynamic_cast<PQLNumber &>(*first_).n;
            bool b = db->isRelationshipExists(rs, std::stoi(stmtNum), second_->str());
            return std::make_unique<BoolResult>(b);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Uses/Mod(stmt, var) -> <int, string>[]
        {
            STMT_ENT_SET res = db->getAllRelationshipsWithFilter(rs, getStmtType(dynamic_cast<Synonym &>(*first_).de));
            return std::make_unique<TableResult>(first_->str(), second_->str(), res);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Uses/Modifies(stmt, _) -> int[]
        {
            STMT_SET res = db->getStmtByRelationshipWithFilter(rs, getStmtType(dynamic_cast<Synonym &>(*first_).de));
            return std::make_unique<TableResult>(first_->str(), res);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT):  // Uses/Modifies(stmt, "x") -> int[]
        {
            STMT_SET res = db->getRelationshipWithFilter(rs,
                                                         second_->str(),
                                                         getStmtType(dynamic_cast<Synonym &>(*first_).de));
            return std::make_unique<TableResult>(first_->str(), res);
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
    auto *synonym1 = dynamic_cast<Synonym *>(first_.get());
    auto *synonym2 = dynamic_cast<Synonym *>(second_.get());
    if (synonym1 != nullptr && (synonym1->de == Synonym::DesignEntity::READ) ||
        first_->tag == PQLToken::Tag::WILDCARD ||
        (synonym2 != nullptr && synonym2->de != Synonym::DesignEntity::VARIABLE)) {
        throw SemanticException();
    }
}

ModifiesS::ModifiesS(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second) :
    StmtEntClause(std::move(first), std::move(second), StmtNameRelationship::Modifies) {
    validateArgs();
}

void ModifiesS::validateArgs() {
    auto *synonym1 = dynamic_cast<Synonym *>(first_.get());
    auto *synonym2 = dynamic_cast<Synonym *>(second_.get());
    if (synonym1 != nullptr && (synonym1->de == Synonym::DesignEntity::PRINT) ||
        first_->tag == PQLToken::Tag::WILDCARD ||
        (synonym2 != nullptr && synonym2->de != Synonym::DesignEntity::VARIABLE)) {
        throw SemanticException();
    }
}
