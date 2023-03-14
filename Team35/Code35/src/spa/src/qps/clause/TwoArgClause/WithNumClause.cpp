#include "WithNumClause.h"

#include <unordered_set>

WithNumClause::WithNumClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second) :
        TwoArgClause(std::move(first), std::move(second)) {
    validateArgs();
}

std::unique_ptr<Result> WithNumClause::evaluate(PKBReader *db) {
    throw std::runtime_error("");
}

bool WithNumClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const WithNumClause *>(&rhs);
    return pRhs != nullptr && TwoArgClause::equal(*pRhs);
}

void WithNumClause::validateArgs() {

}