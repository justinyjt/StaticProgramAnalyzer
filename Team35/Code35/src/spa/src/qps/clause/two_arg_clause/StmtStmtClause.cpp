#include "StmtStmtClause.h"

#include <string>
#include <unordered_set>

StmtStmtClause::StmtStmtClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second,
                               StmtStmtRelationship rs) : TwoArgClause(std::move(first), std::move(second)), rs(rs) {
    validateArgs();
}

std::unique_ptr<Result> StmtStmtClause::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | _ | STMT_NUM> */
    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Parent/Follows(s1, s2) -> <int, int>[]
        {
            // Follows(s, s) or Parents(s, s) does not exist
            if (first->str() == second->str()) {
                if (rs != StmtStmtRelationship::NextStar
                    && rs != StmtStmtRelationship::Affects
                    && rs != StmtStmtRelationship::AffectsStar) {
                    return std::make_unique<BoolResult>(false);
                }
                // Next*(s, s), Affects(s, s), Affects*(s,s)
                STMT_STMT_SET res = db->getAllRelationshipsWithFilter(rs,
                                                                      getStmtType(dynamic_cast<Synonym &>(*first).de));
                return std::make_unique<TableResult>(first->str(), second->str(), res);
            }
            STMT_STMT_SET res = db->getAllRelationshipsWithFilter(rs,
                                                                  getStmtType(dynamic_cast<Synonym &>(*first).de),
                                                                  getStmtType(dynamic_cast<Synonym &>(*second).de));
            return std::make_unique<TableResult>(first->str(), second->str(), res);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::STMT_NUM):  // Parent/Follows(stmt, 5) -> int[]
        {
            std::string num = (dynamic_cast<PQLNumber &>(*second)).n;
            STMT_SET s = db->getRelationshipByStmtWithFilter(rs,
                                                             std::stoi(num),
                                                             getStmtType(dynamic_cast<Synonym &>(*first).de),
                                                             false);
            return std::make_unique<TableResult>(first->str(), s);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Parent/Follows(stmt, _) -> int[]
        {
            STMT_SET s = db->getStmtByRelationshipWithFilter(rs,
                                                             getStmtType(dynamic_cast<Synonym &>(*first).de),
                                                             true);
            return std::make_unique<TableResult>(first->str(), s);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Parent/Follows(1, stmt) -> int[]
        {
            std::string num = (dynamic_cast<PQLNumber &>(*first)).n;
            STMT_SET s = db->getRelationshipByStmtWithFilter(rs,
                                                             std::stoi(num),
                                                             getStmtType(dynamic_cast<Synonym &>(*second).de),
                                                             true);
            return std::make_unique<TableResult>(second->str(), s);;
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::STMT_NUM):  // Parent/Follows(1, 2) -> bool
        {
            std::string firstNum = (dynamic_cast<PQLNumber &>(*first)).n;
            std::string secondNum = (dynamic_cast<PQLNumber &>(*second)).n;
            bool b = db->isRelationshipExists(rs, std::stoi(firstNum), std::stoi(secondNum));
            return std::make_unique<BoolResult>(b);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Parent/Follows(3, _) -> bool
        {
            std::string num = (dynamic_cast<PQLNumber &>(*first)).n;
            STMT_SET s = db->getRelationshipByKey(rs, std::stoi(num));
            return std::make_unique<BoolResult>(!s.empty());
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::SYNONYM):  // Parent/Follows(_, stmt) -> int[]
        {
            STMT_SET s = db->getStmtByRelationshipWithFilter(rs,
                                                             getStmtType(dynamic_cast<Synonym &>(*second).de),
                                                             false);
            return std::make_unique<TableResult>(second->str(), s);
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::STMT_NUM):  // Parent/Follows(_, 3) -> bool
        {
            std::string num = (dynamic_cast<PQLNumber &>(*second)).n;
            STMT_SET s = db->getRelationshipByVal(rs, std::stoi(num));
            return std::make_unique<BoolResult>(!s.empty());
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // Parent/Follows(_, _) -> bool
        {
            return std::make_unique<BoolResult>(db->hasRelationship(rs));
        }
        default:
            throw std::runtime_error("");
    }
}

bool StmtStmtClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const StmtStmtClause *>(&rhs);
    return pRhs != nullptr && rs == pRhs->rs && TwoArgClause::equal(*pRhs);
}

void StmtStmtClause::validateArgs() {
    Synonym *synonym1 = dynamic_cast<Synonym *>(first.get());
    Synonym *synonym2 = dynamic_cast<Synonym *>(second.get());
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
                       isTransitive ? StmtStmtRelationship::NextStar : StmtStmtRelationship::Next) {}

Affects::Affects(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, bool isTransitive) :
        StmtStmtClause(std::move(first), std::move(second),
                       isTransitive ? StmtStmtRelationship::AffectsStar : StmtStmtRelationship::Affects) {}
