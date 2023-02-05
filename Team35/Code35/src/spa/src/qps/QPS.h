#include <list>
#include <string>
#include <vector>
#include "qps/query_parser/QueryParser.h"
#include "qps/clauses/Clause.h"

class QPS {
private:
    QueryParser *queryParser;
    std::list<std::string>& results;
public:
    QPS(std::list<std::string>& results);
    void executeQuery(std::string* queryString);
    std::pair<Synonym, std::vector<Clause>> parseResult = queryParser->parse();
};

