#include "TwoArgClause.h"
#include "qps/pql/PQLToken.h"

TwoArgClause::TwoArgClause(std::shared_ptr<PQLToken> first,
                           std::shared_ptr<PQLToken> second) :first(first), second(second) {}

bool TwoArgClause::equal(const TwoArgClause &rhs) const {
    return *first == *(rhs.first) && *second == *(rhs.second);
}

int TwoArgClause::caseValue() const {
    return 10 * static_cast<int>(first->tag) + static_cast<int>(second->tag);
}
