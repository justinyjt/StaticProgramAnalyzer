#include "catch.hpp"
#include "qps/query_parser/QueryParser.h"
#include "../../TestHelper.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/clause/relationship/Modify.h"
#include "qps/clause/Clause.h"
#include "qps/pql/StatementNumber.h"

TEST_CASE("1. Query parser") {

    std::string query = "variable v; assign a;Select v such that Modifies(1,v)";
    QueryParser queryParser;

    // Perform parsing
    std::pair<Synonym, std::vector<Clause>> parseResult = queryParser.parse(query);

    Synonym synonym = Synonym(Synonym::DesignEntity::VARIABLE, "v");
    requireEqual(Synonym(Synonym::DesignEntity::VARIABLE, "v"), parseResult.first);

    std::vector<Clause> clauses = parseResult.second;
    Modify m(StatementNumber(1), Synonym(Synonym::DesignEntity::VARIABLE, "v"));
    Clause& c = clauses.front();
    requireTrue(dynamic_cast<Clause&>(m)==c);
    //requireEqual(c, c);
}