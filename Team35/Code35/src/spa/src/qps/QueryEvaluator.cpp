#include "qps/QueryEvaluator.h"

#include <vector>

#include "qps/clause_optimiser/ClauseOptimiser.h"
#include "qps/result/IdentityResult.h"
#include "commons/util/SetUtil.h"

QueryEvaluator::QueryEvaluator(PKBReader *db) : db(db) {}

std::unique_ptr<Result> QueryEvaluator::evaluate(const std::unique_ptr<SelectClause> &selectClause,
                                                 const std::vector<std::unique_ptr<OptimisableClause>> &clauses) const {
    const HEADER_SET &selectHeaders = selectClause->getHeaders();
    std::unique_ptr<Result> selectRes = selectClause->evaluate(db);

    // if no clauses, just return select list
    if (clauses.empty()) {
        IdentityResult identRes;
        return selectRes->join(identRes);
    }
    std::vector<std::unique_ptr<Result>> results(clauses.size());
    ClauseOptimiser clauseOptimiser(clauses);

    // clause evaluation
    // individual clause evaluation based on order determined by ClauseOptimiser
    ClauseIndexList clauseEvaluationOrder = clauseOptimiser.getClauseEvaluationOrder();
    bool isAnyResultNull = !evaluateIndividualClauses(clauses, clauseEvaluationOrder, results);
    db->clearCache();
    if (isAnyResultNull) {
        BoolResult falseResult(false);
        return selectRes->join(falseResult);
    }

    // Clause group joining - join evaluated results within each group
    // based on order determined by ClauseOptimiser
    ClauseGroupList clauseGroups = clauseOptimiser.getClauseGroups();
    if (clauseGroups.empty()) {
        IdentityResult identRes;
        return selectRes->join(identRes);
    }

    std::vector<std::unique_ptr<Result>> groupedIntermediateResults(clauseGroups.size());
    bool isAnyIntermediateResultNull =
            !joinInClauseGroups(std::move(results), clauseGroups, groupedIntermediateResults);
    if (isAnyIntermediateResultNull) {
        BoolResult falseResult(false);
        return selectRes->join(falseResult);
    }

    // skip joining of groups if boolean select
    bool isBooleanSelect = selectClause->type_ == SelectClause::SelectType::Boolean;
    if (isBooleanSelect) {
        BoolResult trueResult(true);
        return selectRes->join(trueResult);
    }

    // clean up the grouped results, so that only required columns are kept
    std::vector<std::unique_ptr<Result>>
            cleanedIntermediateResults = cleanIntermediateResults(groupedIntermediateResults,
                                                                  clauseGroups,
                                                                  selectHeaders);

    // if no groups left
    if (cleanedIntermediateResults.empty()) {
        IdentityResult identResult;
        return selectRes->join(identResult);
    }

    // clause grouping - join between groups
    std::unique_ptr<Result> finalClauseResult = joinAllResults(std::move(cleanedIntermediateResults));

    // finally, join with the select clause
    std::unique_ptr<Result> finalRes = selectRes->join(*finalClauseResult);
    return std::move(finalRes);
}

bool QueryEvaluator::evaluateIndividualClauses(const std::vector<std::unique_ptr<OptimisableClause>> &clauses,
                                               const ClauseIndexList &clauseEvaluationOrder,
                                               std::vector<std::unique_ptr<Result>> &results) const {
    // TODO(ngjunkang): evaluate clauses in parallel
    for (ClauseIndex clauseIndex : clauseEvaluationOrder) {
        results[clauseIndex] = clauses[clauseIndex]->evaluate(db);
        if (results[clauseIndex]->isNull()) {
            return false;
        }
    }
    return true;
}

bool QueryEvaluator::joinInClauseGroups(std::vector<std::unique_ptr<Result>> results,
                                        const ClauseGroupList &clauseGroups,
                                        std::vector<std::unique_ptr<Result>> &intermediateResults) {
    for (int i = 0; i < clauseGroups.size(); ++i) {
        const auto &joinOrder = clauseGroups[i].getClauseJoinOrder();
        // join all clauses in the group
        std::unique_ptr<Result> curr = std::move(results[joinOrder.front()]);
        for (int j = 1; j < joinOrder.size(); ++j) {
            std::unique_ptr<Result> next = std::move(results[joinOrder[j]]);
            std::unique_ptr<Result> merged = curr->join(*next);
            curr = std::move(merged);
            if (curr->isNull()) {
                return false;
            }
        }
        intermediateResults[i] = std::move(curr);
    }
    return true;
}

std::vector<std::unique_ptr<Result>> QueryEvaluator::cleanIntermediateResults(
        const std::vector<std::unique_ptr<Result>> &intermediateResults,
        const ClauseGroupList &clauseGroups,
        const HEADER_SET &selectHeaders) {
    std::vector<std::unique_ptr<Result>> cleanedIntermediateResults;
    for (int i = 0; i < clauseGroups.size(); ++i) {
        const auto &groupHeaders = clauseGroups[i].getGroupHeaders();
        bool hasIntersection = SetUtil::hasIntersection(groupHeaders, selectHeaders);
        if (!hasIntersection) {
            continue;
        }
        auto &rawIntermediateResult = dynamic_cast<TableResult &>(*intermediateResults[i]);
        cleanedIntermediateResults.emplace_back(rawIntermediateResult.projectColumns(selectHeaders));
    }
    return std::move(cleanedIntermediateResults);
}

std::unique_ptr<Result> QueryEvaluator::joinAllResults(std::vector<std::unique_ptr<Result>> results) {
    std::unique_ptr<Result> finalResult = std::move(results.front());
    for (int i = 1; i < results.size(); ++i) {
        std::unique_ptr<Result> next = std::move(results[i]);
        std::unique_ptr<Result> merged = finalResult->join(*next);
        finalResult = std::move(merged);
        if (finalResult->isNull()) {
            break;
        }
    }
    return std::move(finalResult);
}
