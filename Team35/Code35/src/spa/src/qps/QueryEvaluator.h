#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "qps/clause/OptimisableClause.h"
#include "qps/clause/select_clause/SelectClause.h"
#include "qps/clause_optimiser/ClauseOptimiser.h"

class QueryEvaluator {
 public:
    explicit QueryEvaluator(PKBReader *);
    [[nodiscard]] std::unique_ptr<Result> evaluate(
            const std::unique_ptr<SelectClause> &selectClause,
            const std::vector<std::unique_ptr<OptimisableClause>> &clauses) const;

 private:
    PKBReader *db;

    bool evaluateIndividualClauses(const std::vector<std::unique_ptr<OptimisableClause>> &clauses,
                                   const ClauseIndexList &clauseEvaluationOrder,
                                   std::vector<std::unique_ptr<Result>> &results) const;

    static bool joinInClauseGroups(std::vector<std::unique_ptr<Result>> results,
                                   const ClauseGroupList &clauseGroups,
                                   std::vector<std::unique_ptr<Result>> &intermediateResults);

    static std::vector<std::unique_ptr<Result>> cleanIntermediateResults(
            const std::vector<std::unique_ptr<Result>> &intermediateResults,
            const ClauseGroupList &clauseGroups,
            const HEADER_SET &selectHeaders);

    static std::unique_ptr<Result> joinAllResults(std::vector<std::unique_ptr<Result>> results);
};
