#include "Uses.h"
#include "qps/pql/StatementNumber.h"

UsesS::UsesS(PQLToken* first, PQLToken* second) : Relationship(first, second) {}

Result* UsesS::evaluate(PKBReader *db) {
    throw std::runtime_error("");
}


bool UsesS::operator==(const Clause& rhs) const {
    return (dynamic_cast<const UsesS*>(&rhs) != NULL) && Clause::equal(rhs);
}
