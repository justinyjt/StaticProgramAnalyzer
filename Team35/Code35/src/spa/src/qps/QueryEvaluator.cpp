#include "qps/QueryEvaluator.h"
#include "qps/result/SelectResult.h"

QueryEvaluator::QueryEvaluator(PKBReader *db) : db(db) {}

std::unique_ptr<Result> QueryEvaluator::evaluate(std::pair<std::unique_ptr<SelectClause>,
        std::vector<std::unique_ptr<Clause>>> clausePair) const {
    std::unique_ptr<SelectClause> selectClause = std::move(clausePair.first);
    std::vector<std::unique_ptr<Clause>> clauses = std::move(clausePair.second);
    std::unique_ptr<Result> selectList;

    // if no clauses, just return select list
    if (clauses.size() == 0) {
        selectList = selectClause->evaluate(db);
        SelectResult s = dynamic_cast<SelectResult&>(*selectList);
        return s.getColsCrossProduct();
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
    selectList = selectClause->evaluate(db);
    std::unique_ptr<Result> finalRes = selectList->join( *curr);
    return std::move(finalRes);
}
