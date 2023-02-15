#include "Modifies.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Ident.h"

ModifiesS::ModifiesS(PQLToken* first, PQLToken* second) : Relationship(first, second) {}

Result* ModifiesS::evaluate(PKBReader *db) {
    switch (first->tag) {
        case PQLToken::Tag::STMT_NUM:
        {
            switch (second->tag) {
                // modifies(1, v), modifies(1, _) -> ident[]
                case PQLToken::Tag::SYNONYM: case PQLToken::Tag::WILDCARD:
                    {
                        ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies,
                                                    (dynamic_cast<const StatementNumber*>(first))->n);
                        Result *result = new StrResult(set);
                        return result;
                    }

                // modifies(1, "x") -> bool
                case PQLToken::Tag::IDENT:
                    {
                        bool b = db->isRelationshipExists(StmtNameRelationship::Modifies,
                                                    (dynamic_cast<const StatementNumber*>(first))->n,
                                                    (dynamic_cast<const Ident*>(second))->s);
                        Result *result = new BoolResult(b);
                        return result;
                    }
            }
        }
        case PQLToken::Tag::SYNONYM:
        {
            // modifies(s, v)
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Modifies);
            return nullptr;
        }
        default:
            throw std::runtime_error("");
    }
}


bool ModifiesS::operator==(const Clause& rhs) const {
    return (dynamic_cast<const ModifiesS*>(&rhs) != NULL) && Clause::equal(rhs);
}

ModifiesP::ModifiesP(PQLToken* first, PQLToken* second) : Relationship(first, second) {}

Result* ModifiesP::evaluate(PKBReader *db) {
    switch (first->tag) {
        case PQLToken::Tag::STMT_NUM:
        {
            switch (second->tag) {
                // modifies(1, v), modifies(1, _) -> ident[]
                case PQLToken::Tag::SYNONYM: case PQLToken::Tag::WILDCARD:
                    {
                        ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies,
                                                    (dynamic_cast<const StatementNumber*>(first))->n);
                        Result *result = new StrResult(set);
                        return result;
                    }

                // modifies(1, "x") -> bool
                case PQLToken::Tag::IDENT:
                    {
                        bool b = db->isRelationshipExists(StmtNameRelationship::Modifies,
                                                    (dynamic_cast<const StatementNumber*>(first))->n,
                                                    (dynamic_cast<const Ident*>(second))->s);
                        Result *result = new BoolResult(b);
                        return result;
                    }
            }
        }
        case PQLToken::Tag::SYNONYM:
        {
            // modifies(s, v)
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Modifies);
            return nullptr;
        }
        default:
            throw std::runtime_error("");
    }
}


bool ModifiesP::operator==(const Clause& rhs) const {
    return (dynamic_cast<const ModifiesP*>(&rhs) != NULL) && Clause::equal(rhs);
}
