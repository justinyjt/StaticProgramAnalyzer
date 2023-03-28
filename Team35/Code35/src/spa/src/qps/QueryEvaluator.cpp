#include "qps/QueryEvaluator.h"
#include "qps/result/SelectResult.h"
#include "qps/result/IdentityResult.h"

QueryEvaluator::QueryEvaluator(PKBReader *db) : db(db) {}

std::unique_ptr<Result> QueryEvaluator::evaluate(std::pair<std::unique_ptr<SelectClause>,
        std::vector<std::unique_ptr<Clause>>> clausePair) const {
    std::unique_ptr<SelectClause> selectClause = std::move(clausePair.first);
    std::vector<std::unique_ptr<Clause>> clauses = std::move(clausePair.second);
    std::unique_ptr<Result> selectRes;

    // if no clauses, just return select list
    if (clauses.size() == 0) {
        selectRes = selectClause->evaluate(db);
        std::unique_ptr<Result> identRes = std::make_unique<IdentityResult>();
        return selectRes->join(*identRes);
    }
    std::unique_ptr<Result> curr = clauses[0]->evaluate(db);
    int i = 1;
    while (i < clauses.size()) {
        std::unique_ptr<Result> next = clauses[i]->evaluate(db);
        std::unique_ptr<Result> merged = curr->join(*next);
        curr = std::move(merged);
        i++;
    }

    // finally, join with the select clause
    selectRes = selectClause->evaluate(db);
    std::unique_ptr<Result> finalRes = selectRes->join(*curr);
    return std::move(finalRes);
}
