#include "Parent.h"
#include "qps/pql/StatementNumber.h"

Parent::Parent(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second, bool isRecursive) :
    TwoArgClause(first, second), isRecursive(isRecursive) {}

std::unique_ptr<Result> Parent::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | _ | STMT_NUM> */

    StmtStmtRelationship rs = isRecursive ? StmtStmtRelationship::ParentStar : StmtStmtRelationship::Parent;

    switch (caseValue()) {
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Parent(s1, s2) -> pair<int, int>[]
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::vector<std::list<std::string>> vec;
            for (auto& p : s)
                vec.push_back({std::to_string(p.first), std::to_string(p.second)});
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), second->str(), vec);
            return std::move(result);
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::STMT_NUM):  // Parent(stmt, 5) -> int[]
        {
            int num = (std::dynamic_pointer_cast<StatementNumber>(second))->n;
            STMT_SET s = db->getRelationshipByVal(rs, num);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), s);
            return std::move(result);
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Parent(stmt, _) -> int[]
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::unordered_set<int> set;
            for (auto& p : s)
                set.insert(p.first);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), set);
            return std::move(result);
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Parent(1, stmt) -> int[]
        {
            int num = (std::dynamic_pointer_cast<StatementNumber>(first))->n;
            STMT_SET s = db->getRelationshipByKey(rs, num);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), s);
            return std::move(result);
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::STMT_NUM):  // Parent(1, 2) -> bool
        {
            int firstNum = (std::dynamic_pointer_cast<StatementNumber>(first))->n;
            int secondNum = (std::dynamic_pointer_cast<StatementNumber>(second))->n;
            bool b = db->isRelationshipExists(rs, firstNum, secondNum);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(b);
            return std::move(result);
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Parent(3, _) -> bool
        {
            int num = (std::dynamic_pointer_cast<StatementNumber>(first))->n;
            STMT_SET s = db->getRelationshipByKey(rs, num);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(s.size() > 0);
            return std::move(result);
        }
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::SYNONYM):  // Parent(_, stmt) -> int[]
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::unordered_set<int> set;
            for (auto& p : s)
                set.insert(p.second);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), set);
            return std::move(result);
        }
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::STMT_NUM):  // Parent(_, 3) -> bool
        {
            int num = (std::dynamic_pointer_cast<StatementNumber>(second))->n;
            STMT_SET s = db->getRelationshipByVal(rs, num);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(s.size() > 0);
            return std::move(result);
        }
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // Parent(_, _) -> bool
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(s.size() > 0);
            return std::move(result);
        }
        default:
            throw std::runtime_error("");
    }
}

bool Parent::operator==(const Clause& rhs) const {
    const Parent* pRhs = dynamic_cast<const Parent*>(&rhs);
    if (pRhs != nullptr) {
        return equal(*pRhs) && isRecursive == pRhs->isRecursive;
    }
    return false;
}
