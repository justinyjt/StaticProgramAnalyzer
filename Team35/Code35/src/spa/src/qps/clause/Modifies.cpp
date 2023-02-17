#include "Modifies.h"

ModifiesS::ModifiesS(PQLToken* first, PQLToken* second) : TwoArgClause(first, second) {}

Result* ModifiesS::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */

    switch (caseValue()) {
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Modifies(1, var) -> string[]
        {
            int stmtNum = (dynamic_cast<const StatementNumber*>(first))->n;
            ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies, stmtNum);
            Result *result = new TableResult(second->str(), set);
            return result;
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Modifies(1, _) -> bool
        {
            int stmtNum = (dynamic_cast<const StatementNumber*>(first))->n;
            ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies, stmtNum);
            Result* result = new BoolResult(set.size() > 0);
            return result;
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::IDENT):  // Modifies(1, "x") -> bool
        {
            int stmtNum = (dynamic_cast<const StatementNumber*>(first))->n;
            bool b = db->isRelationshipExists(StmtNameRelationship::Modifies,
                                            stmtNum, second->str());
            Result *result = new BoolResult(b);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Modifies(stmt, var) -> pair<int, string>[]
        {
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Modifies);
            Result *result = new TableResult(first->str(), second->str(), set);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Modifies(stmt, _) -> int[]
        {
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Modifies);
            ENT_SET eset;
            for (auto& p : set)
                eset.insert(std::to_string(p.first));
            Result *result = new TableResult(first->str(), eset);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT):  // Modifies(stmt, "x")/ -> int[]
        {
            STMT_SET stmtSet = db->getRelationship(StmtNameRelationship::Modifies, second->str());
            Result *result = new TableResult(first->str(), stmtSet);
            return result;
        }
        default:
            throw std::runtime_error("");
    }}


bool ModifiesS::operator==(const Clause& rhs) const {
    const ModifiesS* pRhs = dynamic_cast<const ModifiesS*>(&rhs);
    if (pRhs != nullptr) {
        return equal(*pRhs);
    }
    return false;
}
