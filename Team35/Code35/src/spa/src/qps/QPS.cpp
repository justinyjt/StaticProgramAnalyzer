#include "QPS.h"
#include "pkb/PKBReader.h"

QPS::QPS(PKBReader* pkbReader) {
    db = pkbReader;
    std::unique_ptr<QueryParser> queryParser = std::make_unique<QueryParser>();
};

void QPS::executeQuery(std::string& query, std::list<std::string>& result) {
    std::vector<std::unique_ptr<Clause>> clauses = queryParser->parse(query);

    // first clause
    std::unique_ptr<Result> curr = clauses[0]->evaluate(db);
    int i = 1;
    while (i < clauses.size()) {
        std::unique_ptr<Result> next = clauses[i]->evaluate(db);
        std::unique_ptr<Result> merged = curr->merge(std::move(next));
        curr = std::move(merged);
    }

    curr->output(result);
}
