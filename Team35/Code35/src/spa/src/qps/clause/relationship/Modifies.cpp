#include "Modifies.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Ident.h"

ModifiesS::ModifiesS(PQLToken* first, PQLToken* second) : Relationship(first, second) {}

Result* ModifiesS::evaluate(PKBReader *db) {
    /* <stmt synonym | stmt_num>, <var synonym | ident | _ > */
    switch (first->tag) {
        case PQLToken::Tag::STMT_NUM:
        {
            int stmtNum = (dynamic_cast<const StatementNumber*>(first))->n;
            switch (second->tag) {
                // (stmt_num, v), (stmt_num, _) -> string[]
                case PQLToken::Tag::SYNONYM: case PQLToken::Tag::WILDCARD:
                {
                    ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies, stmtNum);
                    Result *result = new StrResult(set);
                    return result;
                }

                // (stmt_num, ident) -> bool
                case PQLToken::Tag::IDENT:
                {
                    bool b = db->isRelationshipExists(StmtNameRelationship::Modifies, stmtNum,
                                                (dynamic_cast<const Ident*>(second))->str());
                    Result *result = new BoolResult(b);
                    return result;
                }
            }
        }
        case PQLToken::Tag::SYNONYM:
        {
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Modifies);
            switch (second->tag) {
                // (stmt synonym, v), (stmt synonym, _) -> string[]
                case PQLToken::Tag::SYNONYM: 
                {}
                case PQLToken::Tag::WILDCARD:
                {}
                case PQLToken::Tag::IDENT:
                {}
            }
        }
        default:
            throw std::runtime_error("");
    }
}


bool ModifiesS::operator==(const Clause& rhs) const {
    return (dynamic_cast<const ModifiesS*>(&rhs) != NULL) && Clause::equal(rhs);
}
