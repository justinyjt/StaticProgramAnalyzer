#include "StmtEntClause.h"

#include <string>

StmtEntClause::StmtEntClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second,
                             StmtNameRelationship rs) : TwoArgClause(std::move(first), std::move(second)), rs(rs) {}

std::unique_ptr<Result> StmtEntClause::handleNumSyn(PKBReader *db, PQLNumber &first, Synonym &second) {
    ENT_SET set = db->getRelationship(rs, std::stoi(first.str()));
    return std::make_unique<TableResult>(second.str(), set);
}

std::unique_ptr<Result> StmtEntClause::handleNumIdent(PKBReader *db, PQLNumber &first, Ident &second) {
    bool b = db->isRelationshipExists(rs, std::stoi(first.str()), second.str());
    return std::make_unique<BoolResult>(b);
}

std::unique_ptr<Result> StmtEntClause::handleNumWc(PKBReader *db, PQLNumber &first) {
    ENT_SET set = db->getRelationship(rs, std::stoi(first.str()));
    return std::make_unique<BoolResult>(!set.empty());
}

std::unique_ptr<Result> StmtEntClause::handleSynSyn(PKBReader *db, Synonym &first, Synonym &second) {
    STMT_ENT_SET res = db->getAllRelationshipsWithFilter(rs, getStmtType(first.de));
    return std::make_unique<TableResult>(first.str(), second.str(), res);
}

std::unique_ptr<Result> StmtEntClause::handleSynWc(PKBReader *db, Synonym &first) {
    STMT_SET res = db->getStmtByRelationshipWithFilter(rs, getStmtType(first.de));
    return std::make_unique<TableResult>(first.str(), res);
}

std::unique_ptr<Result> StmtEntClause::handleSynIdent(PKBReader *db, Synonym &first, Ident &second) {
    STMT_SET res = db->getRelationshipWithFilter(rs,
                                                 second.str(),
                                                 getStmtType(first.de));
    return std::make_unique<TableResult>(first.str(), res);
}

std::unique_ptr<Result> StmtEntClause::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */
    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Uses/Modifies(1, var) -> string[]
            return handleNumSyn(db, dynamic_cast<PQLNumber &>(*first_), dynamic_cast<Synonym &>(*second_));
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Uses/Modifies(1, _) -> bool
            return handleNumWc(db, dynamic_cast<PQLNumber &>(*first_));
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::IDENT):  // Uses/Modifies(1, "x") -> bool
            return handleNumIdent(db, dynamic_cast<PQLNumber &>(*first_), dynamic_cast<Ident &>(*second_));
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Uses/Mod(stmt, var) -> <int, string>[]
            return handleSynSyn(db, dynamic_cast<Synonym &>(*first_), dynamic_cast<Synonym &>(*second_));
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Uses/Modifies(stmt, _) -> int[]
            return handleSynWc(db, dynamic_cast<Synonym &>(*first_));
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT):  // Uses/Modifies(stmt, "x") -> int[]
            return handleSynIdent(db, dynamic_cast<Synonym &>(*first_), dynamic_cast<Ident &>(*second_));
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
