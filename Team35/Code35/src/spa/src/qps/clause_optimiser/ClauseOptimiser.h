#pragma once

#include <cstdint>
#include <memory>
#include <unordered_set>
#include <vector>

#include "ClauseGroup.h"
#include "qps/clause/OptimisableClause.h"
#include "qps/clause_optimiser/types.h"

class ClauseOptimiser {
 public:
    explicit ClauseOptimiser(const std::vector<std::unique_ptr<OptimisableClause>> &clauses);
    virtual ~ClauseOptimiser() = default;

    ClauseIndexList getClauseEvaluationOrder() const;
    ClauseGroupList getClauseGroups() const;

 private:
    const std::vector<std::unique_ptr<OptimisableClause>> &clauses_;
    bool compareClauseIndex(const ClauseIndex &lhs, const ClauseIndex &rhs) const;
};
