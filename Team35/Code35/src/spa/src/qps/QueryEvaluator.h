#include "qps/clause/Clause.h"
#include "qps/pql/Synonym.h"
#include "pkb/PKB.h"
#include "pkb/PKBReader.h"

class QueryEvaluator {
  public:
    QueryEvaluator(PKB& pkb);
    PKB& pkb;
    PKBReader* pkbReader;
    Result evaluate(Synonym, std::vector<Clause>);
};