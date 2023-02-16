#include "Uses.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Ident.h"

UsesS::UsesS(PQLToken* first, PQLToken* second) : TwoArgClause(first, second) {}

Result* UsesS::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */
    switch (caseValue()) {
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Uses(1, var) -> string[]
        {
            int stmtNum = (dynamic_cast<const StatementNumber*>(first))->n;
            ENT_SET set = db->getRelationship(StmtNameRelationship::Uses, stmtNum);
            Result *result = new OneColResult<std::string>(second->str(), set);
            return result;
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Uses(1, _)/ -> bool
        {
            int stmtNum = (dynamic_cast<const StatementNumber*>(first))->n;
            ENT_SET set = db->getRelationship(StmtNameRelationship::Uses, stmtNum);
            Result* result = new BoolResult(set.size() > 0);
            return result;
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::IDENT):  // Uses(1, "x")/ -> bool
        {
            int stmtNum = (dynamic_cast<const StatementNumber*>(first))->n;
            bool b = db->isRelationshipExists(StmtNameRelationship::Uses, stmtNum,
                                            second->str());
            Result *result = new BoolResult(b);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Uses(stmt, var)/ -> pair<int, string>[]
        {
            std::vector<std::pair<int, std::string>> vec;
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Uses);
            for (auto& se : set) {
                vec.push_back(se);
            }
            Result *result = new TwoColResult<int, std::string>(first->str(), second->str(), vec);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Uses(stmt, _)/ -> int[]
        {
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Uses);
            std::vector<int> vec;
            for (const STMT_ENT& se : set)
                vec.push_back(se.first);
            Result *result = new OneColResult<int>(first->str(), vec);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT):  // Uses(stmt, "x")/ -> int[]
        {
            STMT_SET stmtSet = db->getRelationship(StmtNameRelationship::Uses, second->str());
            Result *result = new OneColResult<int>(first->str(), stmtSet);
            return result;
        }
        default:
            throw std::runtime_error("");
    }
}


bool UsesS::operator==(const Clause& rhs) const {
    const UsesS* pRhs = dynamic_cast<const UsesS*>(&rhs);
    if (pRhs != nullptr) {
        return equal(*pRhs);
    }
    return false;
}
