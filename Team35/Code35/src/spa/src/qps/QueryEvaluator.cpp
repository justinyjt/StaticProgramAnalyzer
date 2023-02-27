#include "qps/QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKBReader *db) : db(db) {}

std::unique_ptr<Result> QueryEvaluator::evaluate(std::vector<std::unique_ptr<Clause>> &clauses) const {
    if (clauses.size() == 1) {
        std::unique_ptr<Result> res = clauses[0]->evaluate(db);
        return std::move(res);
    }

    std::unique_ptr<Result> curr = clauses[1]->evaluate(db);
    int i = 2;
    while (i < clauses.size()) {
        std::unique_ptr<Result> next = clauses[i]->evaluate(db);
        std::unique_ptr<Result> merged = Result::join(*curr, *next);
        curr = std::move(merged);
        i++;
    }

    // finally, join with the select clause
    std::unique_ptr<Result> selectList = clauses[0]->evaluate(db);
    std::unique_ptr<Result> finalRes = Result::join(*selectList, *curr);
    return std::move(finalRes);
}
