#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/clause/Clause.h"
#include "qps/clause/TwoArgClause/StmtEntClause.h"
#include "qps/clause/TwoArgClause/StmtStmtClause.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Expression.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"
#include "qps/query_parser/QueryParser.h"

TEST_CASE("1. Query parser") {
    std::string query = "variable v, x; assign a, b, c; read y; Select c such that Parent*(a,b) pattern a ( _ , _\"x\"_)";
    QueryParser qp;
    std::vector<std::unique_ptr<Clause>> clauses = qp.parse(query);

    requireTrue(clauses.size() == 3);

    std::unique_ptr<Synonym> st = std::make_unique<Synonym>(Synonym::DesignEntity::ASSIGN, "a");
    std::unique_ptr<Synonym> syn = std::make_unique<Synonym>(Synonym::DesignEntity::ASSIGN, "b");
    std::unique_ptr<Parent> m = std::make_unique<Parent>(std::move(st), std::move(syn), true);

    requireTrue(*clauses[1] == *m);

    std::unique_ptr<Wildcard> w = std::make_unique<Wildcard>();
    std::unique_ptr<Expression> expr = std::make_unique<Expression>("x", true);

    std::unique_ptr<AssignPattern> pattern = std::make_unique<AssignPattern>(std::move(w), std::move(expr), "a");

    std::unique_ptr<Clause> c2 = std::move(clauses[2]);
    requireTrue(*c2 == *pattern);
}
