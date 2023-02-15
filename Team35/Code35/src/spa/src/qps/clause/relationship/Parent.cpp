#include "Parent.h"

Parent::Parent(Tok* first, Tok* second) : Relationship(first, second) {
    validateArgs(first, second);
}

Result* Parent::evaluate(PKBReader *db) {
    throw std::runtime_error("");
}

void Parent::validateArgs(Tok* first, Tok* second) {
}

bool Parent::operator==(const Clause& rhs) const {
    return (dynamic_cast<const Parent*>(&rhs) != NULL) && Clause::equal(rhs);
}

ParentT::ParentT(Tok* first, Tok* second) : Relationship(first, second) {
    validateArgs(first, second);
}

Result* ParentT::evaluate(PKBReader *db) {
    throw std::runtime_error("");
}

void ParentT::validateArgs(Tok* first, Tok* second) {

}

bool ParentT::operator==(const Clause& rhs) const {
    return (dynamic_cast<const ParentT*>(&rhs) != NULL) && Clause::equal(rhs);
}
