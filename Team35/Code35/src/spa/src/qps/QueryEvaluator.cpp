#include "qps/QueryEvaluator.h"
#include "qps/clause/select_clause/SingleSynonymSelectClause.h"

QueryEvaluator::QueryEvaluator(PKBReader *db) : db(db) {}

std::unique_ptr<Result> QueryEvaluator::evaluate(std::pair<std::unique_ptr<SelectClause>,
        std::vector<std::unique_ptr<Clause>>> clausePair) const {
    std::unique_ptr<SelectClause> selectClause = std::move(clausePair.first);
    std::vector<std::unique_ptr<Clause>> clauses = std::move(clausePair.second);

    std::unique_ptr<Result> curr = clauses[0]->evaluate(db);
    int i = 1;
    while (i < clauses.size()) {
        std::unique_ptr<Result> next = clauses[i]->evaluate(db);
        std::unique_ptr<Result> merged = Result::join(*curr, *next);
        curr = std::move(merged);
        i++;
    }

    // finally, join with the select clause
    std::unique_ptr<Result> selectList = selectClause->evaluate(db);
    std::unique_ptr<Result> finalRes = Result::selectJoin(*selectList, *curr);
    return std::move(finalRes);
}
