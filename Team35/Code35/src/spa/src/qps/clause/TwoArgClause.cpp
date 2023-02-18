#include "TwoArgClause.h"

#include <utility>
#include "qps/pql/PQLToken.h"

TwoArgClause::TwoArgClause(std::unique_ptr<PQLToken> first,
                           std::unique_ptr<PQLToken> second) :first(std::move(first)), second(std::move(second)) {}

bool TwoArgClause::equal(const TwoArgClause &rhs) const {
    return *first == *(rhs.first) && *second == *(rhs.second);
}

int TwoArgClause::caseValue() const {
    return 10 * static_cast<int>(first->tag) + static_cast<int>(second->tag);
}
