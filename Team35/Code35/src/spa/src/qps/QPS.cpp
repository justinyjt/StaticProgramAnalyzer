#include "QPS.h"

QPS::QPS(PKBReader* pkbReader) {
    db = pkbReader;
    queryParser = new QueryParser();
};

void QPS::executeQuery(std::string& query, std::list<std::string>& result) {
    std::vector<Clause*> clauses = queryParser->parse(query);
    
    // first clause
    Result* curr = clauses[0]->evaluate(db);
    int i = 1;
    while (i < clauses.size()) {
        Result* next = clauses[i]->evaluate(db);
        Result* merged = curr->merge(next);
        delete curr; 
        delete next;
        curr = merged;
    }

    curr->output(result);
}
