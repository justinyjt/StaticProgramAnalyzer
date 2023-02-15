#include "Clause.h"

Clause::Clause(PQLToken* first, PQLToken* second) : first(first), second(second) {}

bool Clause::equal(const Clause &rhs) const {
    return *first == *(rhs.first) && *second == *(rhs.second);
}

bool Clause::operator!=(const Clause &rhs) const {
    return !(*this == rhs);
}

int Clause::caseValue() const {
    return 10 * static_cast<int>(first->tag) + static_cast<int>(second->tag);
}
