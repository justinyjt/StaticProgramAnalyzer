#include "StmtStmtClause.h"

#include <string>
#include <unordered_set>

#include "qps/clause/OptimisableClause.h"

StmtStmtClause::StmtStmtClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second,
                               StmtStmtRelationship rs) : TwoArgClause(std::move(first), std::move(second)), rs(rs) {
    validateArgs();
}

std::unique_ptr<Result> StmtStmtClause::handleSynSynCase(PKBReader *db, Synonym &first, Synonym &second) {
    // Follows(s, s) or Parents(s, s) does not exist
    if (first.str() == second.str()) {
        if (rs != StmtStmtRelationship::NextStar
            && rs != StmtStmtRelationship::Affects
            && rs != StmtStmtRelationship::AffectsStar) {
            return std::make_unique<BoolResult>(false);
        }
        // Next*(s, s), Affects(s, s), Affects*(s,s)
        STMT_STMT_SET res = db->getAllRelationshipsWithFilter(rs,
                                                              getStmtType(first.de));
        return std::make_unique<TableResult>(first.str(), second.str(), res);
    }
    STMT_STMT_SET res = db->getAllRelationshipsWithFilter(rs,
                                                          getStmtType(first.de),
                                                          getStmtType(second.de));
    return std::make_unique<TableResult>(first.str(), second.str(), res);
}

std::unique_ptr<Result> StmtStmtClause::handleSynNumCase(PKBReader *db, Synonym &first, PQLNumber &second) {
    STMT_SET s = db->getRelationshipByStmtWithFilter(rs,
                                                     std::stoi(second.str()),
                                                     getStmtType(first.de),
                                                     ArgType::Value);
    return std::make_unique<TableResult>(first.str(), s);
}

std::unique_ptr<Result> StmtStmtClause::handleSynWcCase(PKBReader *db, Synonym &first) {
    STMT_SET s = db->getStmtByRelationshipWithFilter(rs,
                                                     getStmtType(first.de),
                                                     ArgType::Key);
    return std::make_unique<TableResult>(first.str(), s);
}

std::unique_ptr<Result> StmtStmtClause::handleNumSynCase(PKBReader *db, PQLNumber &first, Synonym &second) {
    STMT_SET s = db->getRelationshipByStmtWithFilter(rs,
                                                     std::stoi(first.str()),
                                                     getStmtType(second.de),
                                                     ArgType::Key);
    return std::make_unique<TableResult>(second.str(), s);
}

std::unique_ptr<Result> StmtStmtClause::handleNumNumCase(PKBReader *db, PQLNumber &first, PQLNumber &second) {
    bool b = db->isRelationshipExists(rs, std::stoi(first.str()), std::stoi(second.str()));
    return std::make_unique<BoolResult>(b);
}

std::unique_ptr<Result> StmtStmtClause::handleNumWcCase(PKBReader *db, PQLNumber &first) {
    STMT_SET s = db->getRelationshipByKey(rs, std::stoi(first.str()));
    return std::make_unique<BoolResult>(!s.empty());
}

std::unique_ptr<Result> StmtStmtClause::handleWcSynCase(PKBReader *db, Synonym &second) {
    STMT_SET s = db->getStmtByRelationshipWithFilter(rs,
                                                     getStmtType(second.de),
                                                     ArgType::Value);
    return std::make_unique<TableResult>(second.str(), s);
}

std::unique_ptr<Result> StmtStmtClause::handleWcNumCase(PKBReader *db, PQLNumber &second) {
    STMT_SET s = db->getRelationshipByVal(rs, std::stoi(second.str()));
    return std::make_unique<BoolResult>(!s.empty());
}


std::unique_ptr<Result> StmtStmtClause::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | _ | STMT_NUM> */
    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Parent/Follows(s1, s2) -> <int, int>[]
            return handleSynSynCase(db, dynamic_cast<Synonym &>(*first_), dynamic_cast<Synonym &>(*second_));
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::STMT_NUM):  // Parent/Follows(stmt, 5) -> int[]
            return handleSynNumCase(db, dynamic_cast<Synonym &>(*first_), dynamic_cast<PQLNumber &>(*second_));
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Parent/Follows(stmt, _) -> int[]
            return handleSynWcCase(db, dynamic_cast<Synonym &>(*first_));
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Parent/Follows(1, stmt) -> int[]
            return handleNumSynCase(db, dynamic_cast<PQLNumber &>(*first_), dynamic_cast<Synonym &>(*second_));
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::STMT_NUM):  // Parent/Follows(1, 2) -> bool
            return handleNumNumCase(db, dynamic_cast<PQLNumber &>(*first_), dynamic_cast<PQLNumber &>(*second_));
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Parent/Follows(3, _) -> bool
            return handleNumWcCase(db, dynamic_cast<PQLNumber &>(*first_));
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::SYNONYM):  // Parent/Follows(_, stmt) -> int[]
            return handleWcSynCase(db, dynamic_cast<Synonym &>(*second_));
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::STMT_NUM):  // Parent/Follows(_, 3) -> bool
            return handleWcNumCase(db, dynamic_cast<PQLNumber &>(*second_));
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // Parent/Follows(_, _) -> bool
            return std::make_unique<BoolResult>(db->hasRelationship(rs));
        default:
            throw std::runtime_error("");
    }
}

bool StmtStmtClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const StmtStmtClause *>(&rhs);
    return pRhs != nullptr && rs == pRhs->rs && TwoArgClause::equal(*pRhs);
}

void StmtStmtClause::validateArgs() {
    auto *synonym1 = dynamic_cast<Synonym *>(first_.get());
    auto *synonym2 = dynamic_cast<Synonym *>(second_.get());
    if (synonym1 != nullptr && (synonym1->de == Synonym::DesignEntity::PROCEDURE
                                || synonym1->de == Synonym::DesignEntity::VARIABLE
                                || synonym1->de == Synonym::DesignEntity::CONSTANT) ||
        synonym2 != nullptr && (synonym2->de == Synonym::DesignEntity::PROCEDURE
                                || synonym2->de == Synonym::DesignEntity::VARIABLE
                                || synonym2->de == Synonym::DesignEntity::CONSTANT)) {
        throw SemanticException();
    }
}

Parent::Parent(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, bool isTransitive) :
        StmtStmtClause(std::move(first), std::move(second),
                       isTransitive ? StmtStmtRelationship::ParentStar : StmtStmtRelationship::Parent) {}

Follows::Follows(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, bool isTransitive) :
        StmtStmtClause(std::move(first), std::move(second),
                       isTransitive ? StmtStmtRelationship::FollowsStar : StmtStmtRelationship::Follows) {}

Next::Next(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, bool isTransitive) :
        StmtStmtClause(std::move(first), std::move(second),
                       isTransitive ? StmtStmtRelationship::NextStar : StmtStmtRelationship::Next) {
    this->setComplexity(OptimisableClause::Complexity::Medium);
}

Affects::Affects(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, bool isTransitive) :
        StmtStmtClause(std::move(first), std::move(second),
                       isTransitive ? StmtStmtRelationship::AffectsStar : StmtStmtRelationship::Affects) {
    this->setComplexity(OptimisableClause::Complexity::High);
}
