#include "catch.hpp"
#include "commons/lexer/ILexer.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/query_parser/QueryParser.h"
#include "../../TestHelper.h"

TEST_CASE("1. Query parser") {

    std::string query = "variable v; assign a; \nSelect s such that Modifies(1, v)\n Select a pattern a(_, \"x + 1\")";
    QueryParser queryParser;

    // Perform parsing
    std::pair<Synonym, std::vector<Clause>> parseResult = queryParser.parse(query);

    Synonym synonym = Synonym(Synonym::DesignEntity::VARIABLE, "s");
    requireEqual(Synonym(Synonym::DesignEntity::VARIABLE, "s"), parseResult.first);
}