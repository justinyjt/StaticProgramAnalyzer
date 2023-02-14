#include "Parent.h"
#include "qps/pql/StatementNumber.h"

Parent::Parent(PQLToken* first, PQLToken* second) : Relationship(first, second) {}

Result* Parent::evaluate(PKBReader *db) {
    throw std::runtime_error("");
}


bool Parent::operator==(const Clause& rhs) const {
    return (dynamic_cast<const Parent*>(&rhs) != NULL) && Clause::equal(rhs);
}
