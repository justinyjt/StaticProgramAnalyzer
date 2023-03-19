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
            if (first->str() == second->str() && rs != StmtStmtRelationship::NextStar) {  // Follows(s, s) or Parents(s, s) does not exist
                return std::move(std::make_unique<BoolResult>(false));
            }
            STMT_STMT_SET s = db->getAllRelationships(rs);
            STMT_SET filterSetByFirst = db->getStatements(getStmtType(dynamic_cast<Synonym &>(*first).de));
            STMT_SET filterSetBySecond = db->getStatements(getStmtType(dynamic_cast<Synonym &>(*second).de));
            std::unique_ptr<Result> intermediateResult1 = std::make_unique<TableResult>(first->str(), filterSetByFirst);
            std::unique_ptr<Result>
                    intermediateResult2 = std::make_unique<TableResult>(second->str(), filterSetBySecond);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), second->str(), s);
            std::unique_ptr<Result> joinedResult = Result::join(*result, *intermediateResult1);
            return std::move(Result::join(*joinedResult, *intermediateResult2));
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::STMT_NUM):  // Parent/Follows(stmt, 5) -> int[]
        {
            std::string num = (dynamic_cast<PQLNumber &>(*second)).n;
            STMT_SET set = db->getRelationshipByVal(rs, std::stoi(num));
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<Synonym &>(*first).de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(first->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), set);
            return std::move(Result::join(*result, *intermediateResult));
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Parent/Follows(stmt, _) -> int[]
        {
            STMT_SET parentStmtSet = db->getKeyStmtByRelationship(rs);
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<Synonym &>(*first).de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(first->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), parentStmtSet);
            return std::move(Result::join(*result, *intermediateResult));
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Parent/Follows(1, stmt) -> int[]
        {
            std::string num = (dynamic_cast<PQLNumber &>(*first)).n;
            STMT_SET set = db->getRelationshipByKey(rs, std::stoi(num));
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<Synonym &>(*second).de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(second->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), set);
            return std::move(Result::join(*result, *intermediateResult));
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::STMT_NUM):  // Parent/Follows(1, 2) -> bool
        {
            std::string firstNum = (dynamic_cast<PQLNumber &>(*first)).n;
            std::string secondNum = (dynamic_cast<PQLNumber &>(*second)).n;
            bool b = db->isRelationshipExists(rs, std::stoi(firstNum), std::stoi(secondNum));
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(b);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Parent/Follows(3, _) -> bool
        {
            std::string num = (dynamic_cast<PQLNumber &>(*first)).n;
            STMT_SET s = db->getRelationshipByKey(rs, std::stoi(num));
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!s.empty());
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::SYNONYM):  // Parent/Follows(_, stmt) -> int[]
        {
            STMT_SET parentStmtSet = db->getValueStmtByRelationship(rs);
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<Synonym &>(*second).de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(second->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), parentStmtSet);
            return std::move(Result::join(*result, *intermediateResult));
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::STMT_NUM):  // Parent/Follows(_, 3) -> bool
        {
            std::string num = (dynamic_cast<PQLNumber &>(*second)).n;
            STMT_SET s = db->getRelationshipByVal(rs, std::stoi(num));
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!s.empty());
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // Parent/Follows(_, _) -> bool
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!s.empty());
            return std::move(result);
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
