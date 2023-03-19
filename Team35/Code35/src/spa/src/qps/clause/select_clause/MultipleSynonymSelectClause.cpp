#include <cassert>
#include <vector>

#include "SelectClause.h"
#include "MultipleSynonymSelectClause.h"

MultipleSynonymSelectClause::MultipleSynonymSelectClause(std::vector<std::unique_ptr<SingleSynonymSelectClause>>
                                                         selectClauses) : selectClauses(std::move(selectClauses)) {}

std::unique_ptr<Result> MultipleSynonymSelectClause::evaluate(PKBReader *db) {
    // loop through synonyms and get tables
    throw std::runtime_error("");
}

bool MultipleSynonymSelectClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const MultipleSynonymSelectClause *>(&rhs);
    return pRhs != nullptr && selectClauses == pRhs->selectClauses;
}
