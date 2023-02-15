#include "Follows.h"
#include "qps/pql/Wildcard.h"
#include "qps/pql/Synonym.h"
#include "qps/query_exceptions/SemanticException.h"

Follows::Follows(Tok* first, Tok* second) : Relationship(first, second) {
    validateArgs(first, second);
}

Result* Follows::evaluate(PKBReader *db) {
    throw std::runtime_error("");
}

void Follows::validateArgs(Tok* first, Tok* second) {
}

bool Follows::operator==(const Clause& rhs) const {
    return (dynamic_cast<const Follows*>(&rhs) != NULL) && Clause::equal(rhs);
}

FollowsT::FollowsT(Tok* first, Tok* second) : Relationship(first, second) {}

Result* FollowsT::evaluate(PKBReader *db) {
    throw std::runtime_error("");
}

void FollowsT::validateArgs(Tok* first, Tok* second) {
    const Wildcard* wildcard = dynamic_cast<const Wildcard*>(first);
    const Synonym* synonym1 = dynamic_cast<const Synonym*>(first);
    const Synonym* synonym2 = dynamic_cast<const Synonym*>(second);
    if (synonym1 != NULL && synonym1->de == Synonym::DesignEntity::PRINT ||
        wildcard != NULL ||
        (synonym2 != NULL && synonym2->de != Synonym::DesignEntity::VARIABLE)) {
        throw SemanticException();
    }
}

bool FollowsT::operator==(const Clause& rhs) const {
    return (dynamic_cast<const FollowsT*>(&rhs) != NULL) && Clause::equal(rhs);
}
