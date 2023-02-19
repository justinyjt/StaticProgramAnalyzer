#pragma once
#include "qps/QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKBReader* db) : db(db) {}

std::unique_ptr<Result> QueryEvaluator::evaluate(std::vector<std::unique_ptr<Clause>>& clauses) const {
    // first clause
    std::unique_ptr<Result> curr = clauses[0]->evaluate(db);
    int i = 1;
    while (i < clauses.size()) {
        std::unique_ptr<Result> next = clauses[i]->evaluate(db);
        std::unique_ptr<Result> merged = Result::join(curr.get(), next.get());
        curr = std::move(merged);
        i++;
    }
    return std::move(curr);
}
