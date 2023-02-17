#include "Follows.h"
#include "qps/pql/StatementNumber.h"

Follows::Follows(PQLToken* first, PQLToken* second, bool isRecursive) :
    TwoArgClause(first, second), isRecursive(isRecursive) {}

Result* Follows::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | _ | STMT_NUM> */

    StmtStmtRelationship rs = isRecursive ?
            StmtStmtRelationship::FollowsStar : StmtStmtRelationship::Follows;

    switch (caseValue()) {
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Follows(s1, s2) -> pair<int, int>[]
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            Result* result = new TableResult(first->str(), second->str(), s);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::STMT_NUM):  // Follows(stmt, 5) -> int[]
        {
            int num = (dynamic_cast<const StatementNumber*>(second))->n;
            STMT_SET s = db->getRelationshipByVal(rs, num);
            Result* result = new TableResult(first->str(), s);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Follows(stmt, _) -> int[]
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::unordered_set<int> set;
            for (auto& p : s)
                set.insert(p.first);
            Result* result = new TableResult(first->str(), set);
            return result;
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Follows(1, stmt) -> int[]
        {
            int num = (dynamic_cast<const StatementNumber*>(first))->n;
            STMT_SET s = db->getRelationshipByKey(rs, num);
            Result* result = new TableResult(second->str(), s);
            return result;
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::STMT_NUM):  // Follows(1, 2) -> bool
        {
            int firstNum = (dynamic_cast<const StatementNumber*>(first))->n;
            int secondNum = (dynamic_cast<const StatementNumber*>(second))->n;
            bool b = db->isRelationshipExists(rs, firstNum, secondNum);
            Result* result = new BoolResult(b);
            return result;
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Follows(3, _) -> bool
        {
            int num = (dynamic_cast<const StatementNumber*>(first))->n;
            STMT_SET s = db->getRelationshipByKey(rs, num);
            Result* result = new BoolResult(s.size() > 0);
            return result;
        }
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::SYNONYM):  // Follows(_, stmt) -> int[]
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            std::unordered_set<int> set;
            for (auto& p : s)
                set.insert(p.second);
            Result* result = new TableResult(second->str(), set);
            return result;
        }
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::STMT_NUM):  // Follows(_, 3) -> bool
        {
            int num = (dynamic_cast<const StatementNumber*>(second))->n;
            STMT_SET s = db->getRelationshipByVal(rs, num);
            Result* result = new BoolResult(s.size() > 0);
            return result;
        }
        case c(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // Follows(_, _) -> bool
        {
            STMT_STMT_SET s = db->getAllRelationships(rs);
            Result* result = new BoolResult(s.size() > 0);
            return result;
        }
        default:
            throw std::runtime_error("");
    }
}


bool Follows::operator==(const Clause& rhs) const {
    const Follows* pRhs = dynamic_cast<const Follows*>(&rhs);
    if (pRhs != nullptr) {
        return equal(*pRhs);
    }
    return false;
}
