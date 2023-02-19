#include "Follows.h"
#include <unordered_set>

Follows::Follows(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, bool isTransitive) :
    TwoArgClause(std::move(first), std::move(second)), isTransitive(isTransitive) {}

std::unique_ptr<Result> Follows::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | _ | STMT_NUM> */

    StmtStmtRelationship rs = isTransitive ?
                              StmtStmtRelationship::FollowsStar : StmtStmtRelationship::Follows;

    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Follows(s1, s2) -> pair<int, int>[]
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            STMT_SET filterSetByFirst = db->getStatements(getStmtType(dynamic_cast<const Synonym *>(first.get())->de));
            STMT_SET
                filterSetBySecond = db->getStatements(getStmtType(dynamic_cast<const Synonym *>(second.get())->de));
            std::unique_ptr<Result> intermediateResult1 = std::make_unique<TableResult>(first->str(), filterSetByFirst);
            std::unique_ptr<Result>
                intermediateResult2 = std::make_unique<TableResult>(second->str(), filterSetBySecond);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), second->str(), s);
            std::unique_ptr<Result> joinedResult = Result::join(result.get(), intermediateResult1.get());
            return std::move(Result::join(joinedResult.get(), intermediateResult2.get()));
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::STMT_NUM):  // Follows(stmt, 5) -> int[]
        {
            int num = (dynamic_cast<StatementNumber *>(second.get()))->n;
            STMT_SET set = db->getRelationshipByVal(rs, num);
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<const Synonym *>(first.get())->de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(first->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), set);
            return std::move(Result::join(result.get(), intermediateResult.get()));
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Follows(stmt, _) -> int[]
        {
            STMT_SET followsStmtSet = db->getStmtByRelationship(rs);
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<const Synonym *>(first.get())->de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(first->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), followsStmtSet);
            return std::move(Result::join(result.get(), intermediateResult.get()));
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Follows(1, stmt) -> int[]
        {
            int num = (dynamic_cast<StatementNumber *>(first.get()))->n;
            STMT_SET set = db->getRelationshipByVal(rs, num);
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<const Synonym *>(second.get())->de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(second->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), set);
            return std::move(Result::join(result.get(), intermediateResult.get()));
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::STMT_NUM):  // Follows(1, 2) -> bool
        {
            int firstNum = (dynamic_cast<const StatementNumber *>(first.get()))->n;
            int secondNum = (dynamic_cast<const StatementNumber *>(second.get()))->n;
            bool b = db->isRelationshipExists(rs, firstNum, secondNum);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(b);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Follows(3, _) -> bool
        {
            int num = (dynamic_cast<const StatementNumber *>(first.get()))->n;
            STMT_SET s = db->getRelationshipByKey(rs, num);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!s.empty());
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::SYNONYM):  // Follows(_, stmt) -> int[]
        {
            STMT_SET followsStmtSet = db->getStmtByRelationship(rs);
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<const Synonym *>(second.get())->de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(second->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), followsStmtSet);
            return std::move(Result::join(result.get(), intermediateResult.get()));
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::STMT_NUM):  // Follows(_, 3) -> bool
        {
            int num = (dynamic_cast<const StatementNumber *>(second.get()))->n;
            STMT_SET s = db->getRelationshipByVal(rs, num);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!s.empty());
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // Follows(_, _) -> bool
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!s.empty());
            return std::move(result);
        }
        default:throw std::runtime_error("");
    }
}

void Follows::validateArgs() {}

bool Follows::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const Follows *>(&rhs);
    return pRhs != nullptr && equal(*pRhs) && isTransitive == pRhs->isTransitive;
}
