#include "Follows.h"

#include <utility>
#include "qps/pql/StatementNumber.h"

Follows::Follows(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, bool isRecursive) :
    TwoArgClause(std::move(first), std::move(second)), isRecursive(isRecursive) {}

std::unique_ptr<Result> Follows::evaluate(PKBReader* db) {
    /* <stmt SYNONYM | _ | STMT_NUM> */

    StmtStmtRelationship rs = isRecursive ?
            StmtStmtRelationship::FollowsStar : StmtStmtRelationship::Follows;

    switch (caseValue()) {
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Follows(s1, s2) -> pair<int, int>[]
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::vector<std::list<std::string>> vec;
            for (auto& p : s)
                vec.push_back({std::to_string(p.first), std::to_string(p.second)});
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), second->str(), vec);
            return std::move(result);
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::STMT_NUM):  // Follows(stmt, 5) -> int[]
        {
            int num = (dynamic_cast<StatementNumber*>(second.get()))->n;
            STMT_SET s = db->getRelationshipByVal(rs, num);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), s);
            return std::move(result);
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Follows(stmt, _) -> int[]
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::unordered_set<int> set;
            for (auto& p : s)
                set.insert(p.first);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), set);
            return std::move(result);
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Follows(1, stmt) -> int[]
        {
            int num = (dynamic_cast<const StatementNumber*>(first.get()))->n;
            STMT_SET s = db->getRelationshipByKey(rs, num);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), s);
            return std::move(result);
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::STMT_NUM):  // Follows(1, 2) -> bool
        {
            int firstNum = (dynamic_cast<const StatementNumber*>(first.get()))->n;
            int secondNum = (dynamic_cast<const StatementNumber*>(second.get()))->n;
            bool b = db->isRelationshipExists(rs, firstNum, secondNum);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(b);
            return std::move(result);
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Follows(3, _) -> bool
        {
            int num = (dynamic_cast<const StatementNumber*>(first.get()))->n;
            STMT_SET s = db->getRelationshipByKey(rs, num);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!s.empty());
            return std::move(result);
        }
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::SYNONYM):  // Follows(_, stmt) -> int[]
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::unordered_set<int> set;
            for (auto& p : s)
                set.insert(p.second);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), set);
            return std::move(result);
        }
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::STMT_NUM):  // Follows(_, 3) -> bool
        {
            int num = (dynamic_cast<const StatementNumber*>(second.get()))->n;
            STMT_SET s = db->getRelationshipByVal(rs, num);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!s.empty());
            return std::move(result);
        }
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // Follows(_, _) -> bool
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!s.empty());
            return std::move(result);
        }
        default:
            throw std::runtime_error("");
    }
}

void Follows::validateArgs() {}

bool Follows::operator==(const Clause& rhs) const {
    const auto* pRhs = dynamic_cast<const Follows*>(&rhs);
    if (pRhs != nullptr) {
        return equal(*pRhs) && isRecursive == pRhs->isRecursive;
    }
    return false;
}
