#include "Follows.h"
#include "qps/pql/StatementNumber.h"

Follows::Follows(PQLToken* first, PQLToken* second) : Relationship(first, second) {}

Result* Follows::evaluate(PKBReader *db) {
    throw std::runtime_error("");
}


bool Follows::operator==(const Clause& rhs) const {
    return (dynamic_cast<const Follows*>(&rhs) != NULL) && Clause::equal(rhs);
}
