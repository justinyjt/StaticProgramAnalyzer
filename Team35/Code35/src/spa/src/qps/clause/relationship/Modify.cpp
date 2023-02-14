#include "Modify.h"
#include "qps/pql/StatementNumber.h"

ModifiesS::ModifiesS(Tok* first, Tok* second) : Relationship(first, second) {}

// select v such that modifies(1, v)

Result* ModifiesS::evaluate(PKBReader *db) {
    if (first->tag == Tok::Tag::STMT_NUM && second->tag == Tok::Tag::SYNONYM) {
        ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies,
                                          (dynamic_cast<const StatementNumber*>(first))->n);

        StrResult *result = new StrResult(set);
        return dynamic_cast<Result *>(result);
    }

    throw std::runtime_error("");
}


bool ModifiesS::operator==(const Clause& rhs) const {
    return (dynamic_cast<const ModifiesS*>(&rhs) != NULL) && Clause::equal(rhs);
}

ModifiesP::ModifiesP(Tok* first, Tok* second) : Relationship(first, second) {}

// select v such that modifies(1, v)

Result* ModifiesP::evaluate(PKBReader *db) {

    throw std::runtime_error("");
}


bool ModifiesP::operator==(const Clause& rhs) const {
    return (dynamic_cast<const ModifiesP*>(&rhs) != NULL) && Clause::equal(rhs);
}
