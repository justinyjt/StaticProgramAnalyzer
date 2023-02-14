#include "Modifies.h"
#include "qps/pql/StatementNumber.h"

ModifiesS::ModifiesS(PQLToken* first, PQLToken* second) : Relationship(first, second) {}

// select v such that modifies(1, v)

Result* ModifiesS::evaluate(PKBReader *db) {
    if (first->tag == PQLToken::Tag::STMT_NUM && second->tag == PQLToken::Tag::SYNONYM) {
        ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies,
                                          (dynamic_cast<const StatementNumber*>(first))->n);

        StrResult *result = new StrResult(set);
        return result;
    }

    throw std::runtime_error("");
}


bool ModifiesS::operator==(const Clause& rhs) const {
    return (dynamic_cast<const ModifiesS*>(&rhs) != NULL) && Clause::equal(rhs);
}
