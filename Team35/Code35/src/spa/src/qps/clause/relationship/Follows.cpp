#include "Follows.h"

Follows::Follows(Tok* first, Tok* second) : Relationship(first, second) {}

Result* Follows::evaluate(PKBReader *db) {

    throw std::runtime_error("");
}


bool Follows::operator==(const Clause& rhs) const {
    return (dynamic_cast<const Follows*>(&rhs) != NULL) && Clause::equal(rhs);
}

FollowsT::FollowsT(Tok* first, Tok* second) : Relationship(first, second) {}

Result* FollowsT::evaluate(PKBReader *db) {

    throw std::runtime_error("");
}


bool FollowsT::operator==(const Clause& rhs) const {
    return (dynamic_cast<const FollowsT*>(&rhs) != NULL) && Clause::equal(rhs);
}