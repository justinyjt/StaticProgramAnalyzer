#include "MockOptimisableClause.h"

MockOptimisableClause::MockOptimisableClause(OptimisableClause::Complexity complexity, const HEADER_SET &headers) {
    this->setComplexity(complexity);
    this->addHeaders(headers);
}

std::unique_ptr<Result> MockOptimisableClause::evaluate(PKBReader *db) {
    throw std::runtime_error("MockOptimisableClause::evaluate() should not be called");
}

bool MockOptimisableClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const OptimisableClause *>(&rhs);
    return pRhs != nullptr && this->getComplexity() == pRhs->getComplexity() &&
        this->getHeaders() == pRhs->getHeaders();
}
