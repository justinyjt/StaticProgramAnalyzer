#include "TwoArgClause.h"

TwoArgClause::TwoArgClause(PQLToken* first, PQLToken* second) : first(first), second(second) {}

bool TwoArgClause::equal(const TwoArgClause &rhs) const {
    return *first == *(rhs.first) && *second == *(rhs.second);
}

int TwoArgClause::getPairEnum() const {
    return 10 * static_cast<int>(first->tag) + static_cast<int>(second->tag);
}
