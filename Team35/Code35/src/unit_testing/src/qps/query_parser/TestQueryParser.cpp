#include "catch.hpp"
#include "qps/pql/StatementNumber.h"
#include "qps/query_parser/QueryParser.h"

TEST_CASE("1. Query parser") {
    std::string query = "variable v, x; assign a, b, c; read y; Select c such that Modifies(2,v) pattern a ( _ , \"x\")";
    QueryParser qp;
    qp.parse(query);
}
