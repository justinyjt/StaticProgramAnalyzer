#include "Uses.h"

UsesS::UsesS(Tok* first, Tok* second) : Relationship(first, second) {}

Result* UsesS::evaluate(PKBReader *db) {

    throw std::runtime_error("");
}


bool UsesS::operator==(const Clause& rhs) const {
    return (dynamic_cast<const UsesS*>(&rhs) != NULL) && Clause::equal(rhs);
}

UsesP::UsesP(Tok* first, Tok* second) : Relationship(first, second) {}

Result* UsesP::evaluate(PKBReader *db) {

    throw std::runtime_error("");
}


bool UsesP::operator==(const Clause& rhs) const {
    return (dynamic_cast<const UsesP*>(&rhs) != NULL) && Clause::equal(rhs);
}