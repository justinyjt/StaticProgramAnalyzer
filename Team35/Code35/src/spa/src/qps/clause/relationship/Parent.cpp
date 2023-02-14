#include "Parent.h"

Parent::Parent(Tok* first, Tok* second) : Relationship(first, second) {}

Result* Parent::evaluate(PKBReader *db) {

    throw std::runtime_error("");
}


bool Parent::operator==(const Clause& rhs) const {
    return (dynamic_cast<const Parent*>(&rhs) != NULL) && Clause::equal(rhs);
}

ParentT::ParentT(Tok* first, Tok* second) : Relationship(first, second) {}

Result* ParentT::evaluate(PKBReader *db) {

    throw std::runtime_error("");
}


bool ParentT::operator==(const Clause& rhs) const {
    return (dynamic_cast<const ParentT*>(&rhs) != NULL) && Clause::equal(rhs);
}