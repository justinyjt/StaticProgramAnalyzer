#include "Parent.h"

#include <unordered_set>

Parent::Parent(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, bool isTransitive) :
    TwoArgClause(std::move(first), std::move(second)), isTransitive(isTransitive) {}

std::unique_ptr<Result> Parent::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | _ | STMT_NUM> */

    StmtStmtRelationship rs = isTransitive ? StmtStmtRelationship::ParentStar : StmtStmtRelationship::Parent;

    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Parent(s1, s2) -> pair<int, int>[]
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            STMT_SET filterSetByFirst = db->getStatements(getStmtType(dynamic_cast<Synonym&>(*first).de));
            STMT_SET
                filterSetBySecond = db->getStatements(getStmtType(dynamic_cast<Synonym&>(*second).de));
            std::unique_ptr<Result> intermediateResult1 = std::make_unique<TableResult>(first->str(), filterSetByFirst);
            std::unique_ptr<Result>
                intermediateResult2 = std::make_unique<TableResult>(second->str(), filterSetBySecond);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), second->str(), s);
            std::unique_ptr<Result> joinedResult = Result::join(*result, *intermediateResult1);
            return std::move(Result::join(*joinedResult, *intermediateResult2));
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::STMT_NUM):  // Parent(stmt, 5) -> int[]
        {
            int num = (dynamic_cast<StatementNumber&>(*second)).n;
            STMT_SET set = db->getRelationshipByVal(rs, num);
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<Synonym&>(*first).de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(first->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), set);
            return std::move(Result::join(*result, *intermediateResult));
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Parent(stmt, _) -> int[]
        {
            STMT_SET parentStmtSet = db->getKeyStmtByRelationship(rs);
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<Synonym&>(*first).de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(first->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), parentStmtSet);
            return std::move(Result::join(*result, *intermediateResult));
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Parent(1, stmt) -> int[]
        {
            int num = (dynamic_cast<StatementNumber&>(*first)).n;
            STMT_SET set = db->getRelationshipByKey(rs, num);
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<Synonym&>(*second).de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(second->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), set);
            return std::move(Result::join(*result, *intermediateResult));
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::STMT_NUM):  // Parent(1, 2) -> bool
        {
            int firstNum = (dynamic_cast<StatementNumber&>(*first)).n;
            int secondNum = (dynamic_cast<StatementNumber&>(*second)).n;
            bool b = db->isRelationshipExists(rs, firstNum, secondNum);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(b);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Parent(3, _) -> bool
        {
            int num = (dynamic_cast<StatementNumber&>(*first)).n;
            STMT_SET s = db->getRelationshipByKey(rs, num);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!s.empty());
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::SYNONYM):  // Parent(_, stmt) -> int[]
        {
            STMT_SET parentStmtSet = db->getValueStmtByRelationship(rs);
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<Synonym&>(*second).de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(second->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), parentStmtSet);
            return std::move(Result::join(*result, *intermediateResult));
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::STMT_NUM):  // Parent(_, 3) -> bool
        {
            int num = (dynamic_cast<StatementNumber&>(*second)).n;
            STMT_SET s = db->getRelationshipByVal(rs, num);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!s.empty());
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // Parent(_, _) -> bool
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!s.empty());
            return std::move(result);
        }
        default:throw std::runtime_error("");
    }
}

void Parent::validateArgs() {}

bool Parent::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const Parent *>(&rhs);
    return pRhs != nullptr && equal(*pRhs) && isTransitive == pRhs->isTransitive;
}
