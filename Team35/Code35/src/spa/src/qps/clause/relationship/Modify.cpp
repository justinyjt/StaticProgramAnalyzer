#include "Modify.h"
#include "qps/pql/StatementNumber.h"

Modify::Modify(Tok* first, Tok* second) : Relationship(first, second) {}

// select v such that modifies(1, v)

Result* Modify::evaluate(PKBReader *db) {
    if (first->tag == Tok::Tag::STMT_NUM && second->tag == Tok::Tag::SYNONYM) {
        ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies,
                                          (dynamic_cast<const StatementNumber*>(first))->n);

        StrResult *result = new StrResult(set);
        return dynamic_cast<Result *>(result);
    }

    throw std::runtime_error("");
}


bool Modify::operator==(const Clause& rhs) const {
    return (dynamic_cast<const Modify*>(&rhs) != NULL) && Clause::equal(rhs);
}
