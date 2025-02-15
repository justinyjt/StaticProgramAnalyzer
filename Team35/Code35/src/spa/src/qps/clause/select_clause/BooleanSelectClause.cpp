#include "SelectClause.h"
#include "BooleanSelectClause.h"

BooleanSelectClause::BooleanSelectClause() : SelectClause(SelectType::Boolean) {}

std::unique_ptr<Result> BooleanSelectClause::evaluate(PKBReader *db) {
    return std::make_unique<BoolResult>(true);
}

bool BooleanSelectClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const BooleanSelectClause *>(&rhs);
    return pRhs != nullptr;
}
