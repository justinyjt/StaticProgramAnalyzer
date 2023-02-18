#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/clause/Clause.h"
#include "qps/clause/Modifies.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Expression.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"
#include "qps/query_parser/QueryParser.h"
#include "qps/clause/Parent.h"

TEST_CASE("1. Query parser") {
    std::string query = "variable v, x; assign a, b, c; read y; Select c such that Parent*(a,b) pattern a ( _ , _\"x\"_)";
    QueryParser qp;
    std::vector<std::unique_ptr<Clause>> clauses = qp.parse(query);

    requireTrue(clauses.size() == 3);

    std::shared_ptr<Synonym> st = std::make_shared<Synonym>(Synonym::DesignEntity::STMT, "a");
    std::shared_ptr<Synonym> syn = std::make_shared<Synonym>(Synonym::DesignEntity::STMT, "b");
    std::unique_ptr<Parent> m = std::make_unique<Parent>(st,syn, true);

    requireTrue(*clauses[1] == *m);

    std::shared_ptr<Wildcard> w = std::make_shared<Wildcard>();
    std::shared_ptr<Expression> expr = std::make_shared<Expression>("x", true);

    std::unique_ptr<Pattern> pattern = std::make_unique<Pattern>(w, expr, "a");

    std::unique_ptr<Clause> c2 = std::move(clauses[2]);
    requireTrue(*c2 == *pattern);
}
