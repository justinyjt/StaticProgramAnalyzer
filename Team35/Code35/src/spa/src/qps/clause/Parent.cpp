#include "Parent.h"
#include "qps/pql/StatementNumber.h"

Parent::Parent(PQLToken* first, PQLToken* second, bool isRecursive) :
    TwoArgClause(first, second), isRecursive(isRecursive) {}

Result* Parent::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | _ | STMT_NUM> */

    StmtStmtRelationship rs = isRecursive ? StmtStmtRelationship::ParentStar : StmtStmtRelationship::Parent;

    switch (caseValue()) {
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Parent(s1, s2) -> pair<int, int>[]
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::vector<std::pair<int, int>> vec;
            for (auto& p : s)
                vec.push_back(p);
            Result* result = new TwoColResult<int, int>(first->str(), second->str(), vec);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::STMT_NUM):  // Parent(stmt, 5) -> int[]
        {
            int num = (dynamic_cast<const StatementNumber*>(second))->n;
            STMT_SET s = db->getRelationshipByVal(rs, num);
            Result* result = new OneColResult<int>(first->str(), s);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Parent(stmt, _) -> int[]
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::unordered_set<int> set;
            for (auto& p : s)
                set.insert(p.first);
            Result* result = new OneColResult<int>(first->str(), set);
            return result;
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Parent(1, stmt) -> int[]
        {
            int num = (dynamic_cast<const StatementNumber*>(first))->n;
            STMT_SET s = db->getRelationshipByKey(rs, num);
            Result* result = new OneColResult<int>(second->str(), s);
            return result;
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::STMT_NUM):  // Parent(1, 2) -> bool
        {
            int firstNum = (dynamic_cast<const StatementNumber*>(first))->n;
            int secondNum = (dynamic_cast<const StatementNumber*>(second))->n;
            bool b = db->isRelationshipExists(rs, firstNum, secondNum);
            Result* result = new BoolResult(b);
            return result;
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Parent(3, _) -> bool
        {
            int num = (dynamic_cast<const StatementNumber*>(first))->n;
            STMT_SET s = db->getRelationshipByKey(rs, num);
            Result* result = new BoolResult(s.size() > 0);
            return result;
        }
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::SYNONYM):  // Parent(_, stmt) -> int[]
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::unordered_set<int> set;
            for (auto& p : s)
                set.insert(p.second);
            Result* result = new OneColResult<int>(second->str(), set);
            return result;
        }
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::STMT_NUM):  // Parent(_, 3) -> bool
        {
            int num = (dynamic_cast<const StatementNumber*>(second))->n;
            STMT_SET s = db->getRelationshipByVal(rs, num);
            Result* result = new BoolResult(s.size() > 0);
            return result;
        }
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // Parent(_, _) -> bool
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            Result* result = new BoolResult(s.size() > 0);
            return result;
        }
        default:
            throw std::runtime_error("");
    }
}

bool Parent::operator==(const Clause& rhs) const {
    const Parent* pRhs = dynamic_cast<const Parent*>(&rhs);
    if (pRhs != nullptr) {
        return equal(*pRhs);
    }
    return false;
}
