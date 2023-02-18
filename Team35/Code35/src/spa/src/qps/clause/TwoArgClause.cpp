#include "TwoArgClause.h"

TwoArgClause::TwoArgClause(std::unique_ptr<PQLToken> first,
                           std::unique_ptr<PQLToken> second) :first(std::move(first)), second(std::move(second)) {}

bool TwoArgClause::equal(const TwoArgClause &rhs) const {
    return *first == *(rhs.first) && *second == *(rhs.second);
}

int TwoArgClause::getPairEnum() const {
    return 10 * static_cast<int>(first->tag) + static_cast<int>(second->tag);
}
