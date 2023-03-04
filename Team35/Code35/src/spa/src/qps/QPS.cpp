#include "QPS.h"

#include "qps/clause/SelectClause.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/query_exceptions/SemanticException.h"
#include "qps/clause/SingleSynonymSelectClause.h"

QPS::QPS(PKBReader *pkbReader)
    : queryEvaluator(std::make_unique<QueryEvaluator>(pkbReader)), queryParser(std::make_unique<QueryParser>()) {}

void QPS::executeQuery(std::string &query, std::list<std::string> &result) {
    try {
        std::vector<std::unique_ptr<Clause>> clauses = queryParser->parse(query);
        std::string selected = dynamic_cast<SingleSynonymSelectClause&>(*(clauses[0])).syn.ident;
        std::unique_ptr<Result> eval = queryEvaluator->evaluate(clauses);
        eval->output(result, selected);
    } catch (SyntaxException &e) {
        result.emplace_back(e.what());
    } catch (SemanticException &e) {
        result.emplace_back(e.what());
    }
}
