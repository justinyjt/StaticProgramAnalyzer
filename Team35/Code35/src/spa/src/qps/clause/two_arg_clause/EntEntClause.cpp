#include "EntEntClause.h"

EntEntClause::EntEntClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second,
                           NameNameRelationship rs) : TwoArgClause(std::move(first), std::move(second)), rs(rs) {}

std::unique_ptr<Result> EntEntClause::handleSynSyn(
    PKBReader *db, const std::pair<ENT_NAME, ENT_NAME> &argPair) {
    if (argPair.first == argPair.second) {  // Calls(p, p) cannot self call
        return std::move(std::make_unique<BoolResult>(false));
    }
    ENT_ENT_SET s = db->getAllRelationships(rs);
    return std::make_unique<TableResult>(argPair.first, argPair.second, s);
}

std::unique_ptr<Result> EntEntClause::handleSynIdent(
    PKBReader *db, const std::pair<ENT_NAME, ENT_NAME> &argPair) {
    ENT_NAME entName = argPair.second;
    ENT_SET set = db->getRelationshipByVal(rs, entName);
    return std::make_unique<TableResult>(argPair.first, set);
}

std::unique_ptr<Result> EntEntClause::handleSynWc(PKBReader *db, const ENT_NAME &synName) {
    ENT_SET keySet = db->getKeyNameByRelationship(rs);
    return std::make_unique<TableResult>(synName, keySet);
}

std::unique_ptr<Result> EntEntClause::handleIdentSyn(
    PKBReader *db, const std::pair<ENT_NAME, ENT_NAME> &argPair) {
    ENT_NAME entName = argPair.first;
    ENT_SET set = db->getRelationshipByKey(rs, entName);
    return std::make_unique<TableResult>(argPair.second, set);
}

std::unique_ptr<Result> EntEntClause::handleIdentIdent(
    PKBReader *db, const std::pair<ENT_NAME, ENT_NAME> &argPair) {
    ENT_NAME firstEnt = argPair.first;
    ENT_NAME secondEnt = argPair.second;
    bool b = db->isRelationshipExists(rs, firstEnt, secondEnt);
    return std::make_unique<BoolResult>(b);
}

std::unique_ptr<Result> EntEntClause::handleIdentWc(PKBReader *db, const ENT_NAME &entName) {
    ENT_SET s = db->getRelationshipByKey(rs, entName);
    return std::make_unique<BoolResult>(!s.empty());
}

std::unique_ptr<Result> EntEntClause::handleWcSyn(PKBReader *db, const ENT_NAME &synName) {
    ENT_SET callsProcSet = db->getValueNameByRelationship(rs);
    return std::make_unique<TableResult>(synName, callsProcSet);
}

std::unique_ptr<Result> EntEntClause::handleWcIdent(PKBReader *db, const ENT_NAME &entName) {
    ENT_SET s = db->getRelationshipByVal(rs, entName);
    return std::make_unique<BoolResult>(!s.empty());
}

std::unique_ptr<Result> EntEntClause::handleWcWc(PKBReader *db) {
    ENT_ENT_SET s = db->getAllRelationships(rs);
    return std::make_unique<BoolResult>(!s.empty());
}

std::unique_ptr<Result> EntEntClause::evaluate(PKBReader *db) {
    /* <SYNONYM | IDENT | _ >, <SYNONYM | IDENT | _ > */
    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Uses/Modifies(f, v) -> <str, str>[]
            return handleSynSyn(db, std::make_pair(first_->str(), second_->str()), rs);
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT):  // Uses/Modifies(f, "x") -> str[]
            return handleSynIdent(db, std::make_pair(first_->str(), second_->str()), rs);
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Uses/Modifies(f, _) -> str[]
            return handleSynWc(db, first_->str(), rs);
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::SYNONYM):  // Uses/Mod("f", v) -> str[]
            return handleIdentSyn(db, std::make_pair(first_->str(), second_->str()), rs);
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::IDENT):  // Uses/Modifies("f", "x") -> bool
            return handleIdentIdent(db, std::make_pair(first_->str(), second_->str()), rs);
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::WILDCARD):  // Uses/Modifies("f", _) -> bool
            return handleIdentWc(db, first_->str(), rs);
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::SYNONYM):  // Calls(_, f) -> str[]
            return handleWcSyn(db, second_->str(), rs);
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::IDENT):  // Calls(_, "f") -> bool
            return handleWcIdent(db, second_->str(), rs);
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // Calls(_, _) -> bool
            return handleWcWc(db, rs);
        default:
            throw std::runtime_error("");
    }
}

bool EntEntClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const EntEntClause *>(&rhs);
    return pRhs != nullptr && rs == pRhs->rs && TwoArgClause::equal(*pRhs);
}

UsesP::UsesP(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second) :
    EntEntClause(std::move(first), std::move(second), NameNameRelationship::Uses) {
    validateArgs();
}

void UsesP::validateArgs() {
    if (dynamic_cast<Wildcard *>(first_.get()) != nullptr) throw SemanticException();
    auto *synonym1 = dynamic_cast<Synonym *>(first_.get());
    auto *synonym2 = dynamic_cast<Synonym *>(second_.get());
    if (synonym1 != nullptr && synonym1->de != Synonym::DesignEntity::PROCEDURE) {
        throw SemanticException();
    }
    if (synonym2 != nullptr && synonym2->de != Synonym::DesignEntity::VARIABLE) {
        throw SemanticException();
    }
}

ModifiesP::ModifiesP(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second) :
    EntEntClause(std::move(first), std::move(second), NameNameRelationship::Modifies) {
    validateArgs();
}

void ModifiesP::validateArgs() {
    if (dynamic_cast<Wildcard *>(first_.get()) != nullptr) throw SemanticException();
    Synonym *synonym1 = dynamic_cast<Synonym *>(first_.get());
    Synonym *synonym2 = dynamic_cast<Synonym *>(second_.get());
    if (synonym1 != nullptr && synonym1->de != Synonym::DesignEntity::PROCEDURE) {
        throw SemanticException();
    }
    if (synonym2 != nullptr && synonym2->de != Synonym::DesignEntity::VARIABLE) {
        throw SemanticException();
    }
}

Calls::Calls(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, bool isTransitive) :
    EntEntClause(std::move(first), std::move(second),
                 isTransitive ? NameNameRelationship::CallsStar : NameNameRelationship::Calls) {
    validateArgs();
}

void Calls::validateArgs() {
    auto *synonym1 = dynamic_cast<Synonym *>(first_.get());
    auto *synonym2 = dynamic_cast<Synonym *>(second_.get());
    if ((synonym1 != nullptr && synonym1->de != Synonym::DesignEntity::PROCEDURE) ||
        (synonym2 != nullptr && synonym2->de != Synonym::DesignEntity::PROCEDURE)) {
        throw SemanticException();
    }
}
