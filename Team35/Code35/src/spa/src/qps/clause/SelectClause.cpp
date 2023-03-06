#include "SelectClause.h"

SelectClause::SelectClause() {}

bool SelectClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const SelectClause *>(&rhs);
    return pRhs != nullptr;
}
