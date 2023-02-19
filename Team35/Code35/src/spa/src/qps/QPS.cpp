#include "QPS.h"
#include "qps/clause/SelectClause.h"

QPS::QPS(PKBReader* pkbReader) {
    queryEvaluator = std::make_unique<QueryEvaluator>(pkbReader);
    queryParser = std::make_unique<QueryParser>();
};

void QPS::executeQuery(std::string& query, std::list<std::string>& result) {
    std::vector<std::unique_ptr<Clause>> clauses = queryParser->parse(query);
    std::string selected = dynamic_cast<SelectClause*>(clauses[0].get())->syn.ident;
    std::unique_ptr<Result> eval = queryEvaluator->evaluate(clauses);
    eval->output(result, selected);
}
