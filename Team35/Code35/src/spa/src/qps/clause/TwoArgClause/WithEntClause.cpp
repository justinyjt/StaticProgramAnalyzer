#include "WithEntClause.h"

#include <unordered_set>

WithEntClause::WithEntClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second) :
        TwoArgClause(std::move(first), std::move(second)) {
    validateArgs();
}

std::unique_ptr<Result> WithEntClause::evaluate(PKBReader *db) {
    throw std::runtime_error("");
}

bool WithEntClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const WithEntClause *>(&rhs);
    return pRhs != nullptr && TwoArgClause::equal(*pRhs);
}

void WithEntClause::validateArgs() {

}
