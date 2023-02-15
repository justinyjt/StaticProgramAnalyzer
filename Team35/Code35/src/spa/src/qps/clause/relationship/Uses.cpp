#include "Uses.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Ident.h"

UsesS::UsesS(PQLToken* first, PQLToken* second) : Relationship(first, second) {}

Result* UsesS::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */
    switch (caseValue()) {
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Uses(1, var) -> string[]
        {
            int stmtNum = (dynamic_cast<const StatementNumber*>(first))->n;

            std::string synonymIdent = (dynamic_cast<const Synonym*>(second))->ident;
            ENT_SET set = db->getRelationship(StmtNameRelationship::Uses, stmtNum);
            Result *result = new OneColResult<std::string>(synonymIdent, set);
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
                                        (dynamic_cast<const Ident*>(second))->str());
            Result *result = new BoolResult(b);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Uses(stmt, var)/ -> pair<int, string>[]
        {
            std::string synonymIdent = (dynamic_cast<const Synonym*>(first))->ident;
            std::vector<std::pair<int, std::string>> vec;
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Uses);
            for (auto& se : set) {
                vec.push_back(se);
            }
            Result *result = new TwoColResult<int, std::string>(first->str(), synonymIdent, vec);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Uses(stmt, _)/ -> int[]
        {
            std::string synonymIdent = (dynamic_cast<const Synonym*>(first))->ident;
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Uses);
            std::vector<int> vec;
            for (const STMT_ENT& se : set)
                vec.push_back(se.first);
            Result *result = new OneColResult<int>(synonymIdent, vec);
            return result;
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT):  // Uses(stmt, "x")/ -> int[]
        {
            std::string synonymIdent = (dynamic_cast<const Synonym*>(first))->ident;
            STMT_SET stmtSet = db->getRelationship(StmtNameRelationship::Uses, second->str());
            Result *result = new OneColResult<int>(synonymIdent, stmtSet);
            return result;
        }
        default:
            throw std::runtime_error("");
    }
}


bool UsesS::operator==(const Clause& rhs) const {
    return (dynamic_cast<const UsesS*>(&rhs) != NULL) && Clause::equal(rhs);
}
