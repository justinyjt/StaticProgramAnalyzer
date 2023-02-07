#include "catch.hpp"
#include "qps/query_parser/QueryParser.h"
#include "../../TestHelper.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/clause/relationship/Modify.h"
#include "qps/clause/Clause.h"
#include "qps/pql/StatementNumber.h"
#include "qps/clause/pattern/Pattern.h"
#include "qps/pql/Wildcard.h"
#include "qps/pql/ExpressionStr.h"

TEST_CASE("1. Query parser") {

    std::string query = "variable v; assign a;Select v such that Modifies(1,v) pattern a ( _ , \"x + 1\")";
    QueryParser queryParser;

    // Perform parsing
    std::pair<Synonym, std::vector<Clause *>> parseResult = queryParser.parse(query);

    requireEqual(Synonym(Synonym::DesignEntity::VARIABLE, "v"), parseResult.first);

    std::vector<Clause *> clauses = parseResult.second;

    StatementNumber* st = new StatementNumber(1);
    Synonym* syn = new Synonym(Synonym::DesignEntity::VARIABLE, "v");
    Modify m(st, syn);

    Wildcard* w = new Wildcard();
    ExpressionStr* expr = new ExpressionStr("x+1");
    Pattern a(w, expr);

    Clause *c1 = clauses.front();
    requireTrue(c1->first == m.first);
    requireTrue(c1->second == m.second);

    Clause *c2 = clauses.back();
    requireTrue(c2->first == a.first);
    requireTrue(c2->second == a.second);
}
