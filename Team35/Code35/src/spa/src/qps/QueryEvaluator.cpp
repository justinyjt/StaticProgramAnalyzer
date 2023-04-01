#include "qps/QueryEvaluator.h"

#include <vector>

#include "qps/clause_optimiser/ClauseOptimiser.h"
#include "qps/result/IdentityResult.h"
#include "commons/util/SetUtil.h"

QueryEvaluator::QueryEvaluator(PKBReader *db) : db(db) {}

std::unique_ptr<Result> QueryEvaluator::evaluate(std::unique_ptr<SelectClause> selectClause,
                                                 std::vector<std::unique_ptr<OptimisableClause>> clauses) const {
    bool isBooleanSelect = selectClause->type_ == SelectClause::SelectType::Boolean;
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
    bool isAnyClauseEmpty = false;
    bool hasHeaders = false;
    // TODO(ngjunkang): evaluate clauses in parallel
    for (ClauseIndex clauseIndex : clauseEvaluationOrder) {
        results[clauseIndex] = clauses[clauseIndex]->evaluate(db);
        hasHeaders = hasHeaders || !clauses[clauseIndex]->getHeaders().empty();
        if (results[clauseIndex]->isNull()) {
            isAnyClauseEmpty = true;
            break;
        }
    }
    db->clearCache();
    if (isAnyClauseEmpty) {
        BoolResult falseResult(false);
        return selectRes->join(falseResult);
    }

    if (!hasHeaders) {
        BoolResult trueResult(true);
        return selectRes->join(trueResult);
    }

    // Clause group joining - join evaluated results within each group
    // based on order determined by ClauseOptimiser
    ClauseGroupList clauseGroups = clauseOptimiser.getClauseGroups();
    if (clauseGroups.empty()) {
        BoolResult falseResult(false);
        return selectRes->join(falseResult);
    }

    std::vector<std::unique_ptr<Result>> groupedIntermediateResults(clauseGroups.size());
    bool isAnyClauseGroupEmpty = false;
    for (int i = 0; i < clauseGroups.size(); ++i) {
        const auto &joinOrder = clauseGroups[i].getClauseJoinOrder();
        // join all clauses in the group
        std::unique_ptr<Result> curr = std::move(results[joinOrder.front()]);
        for (int j = 1; j < joinOrder.size(); ++j) {
            std::unique_ptr<Result> next = std::move(results[joinOrder[j]]);
            std::unique_ptr<Result> merged = curr->join(*next);
            curr = std::move(merged);
            if (curr->isNull()) {
                isAnyClauseGroupEmpty = true;
                break;
            }
        }

        if (isAnyClauseGroupEmpty) {
            break;
        }
        groupedIntermediateResults[i] = std::move(curr);
    }

    if (isAnyClauseGroupEmpty) {
        BoolResult falseResult(false);
        return selectRes->join(falseResult);
    }

    // skip joining of groups if boolean select
    if (isBooleanSelect) {
        BoolResult trueResult(true);
        return selectRes->join(trueResult);
    }

    // clean up the grouped results, so that only required columns are kept
    std::vector<std::unique_ptr<Result>> cleanedIntermediateResults;
    for (int i = 0; i < clauseGroups.size(); ++i) {
        const auto &groupHeaders = clauseGroups[i].getGroupHeaders();
        bool hasIntersection = SetUtil::hasIntersection(groupHeaders, selectHeaders);
        if (!hasIntersection) {
            continue;
        }
        auto &rawIntermediateResult = dynamic_cast<TableResult &>(*groupedIntermediateResults[i]);
        cleanedIntermediateResults.emplace_back(rawIntermediateResult.projectColumns(selectHeaders));
    }

    // if no groups left
    if (cleanedIntermediateResults.empty()) {
        IdentityResult identResult;
        return selectRes->join(identResult);
    }

    // clause grouping - join between groups
    std::unique_ptr<Result> finalClauseResult = std::move(cleanedIntermediateResults.front());
    for (int i = 1; i < cleanedIntermediateResults.size(); ++i) {
        std::unique_ptr<Result> next = std::move(cleanedIntermediateResults[i]);
        std::unique_ptr<Result> merged = finalClauseResult->join(*next);
        finalClauseResult = std::move(merged);
        if (finalClauseResult->isNull()) {
            break;
        }
    }

    // finally, join with the select clause
    std::unique_ptr<Result> finalRes = selectRes->join(*finalClauseResult);
    return std::move(finalRes);
}
