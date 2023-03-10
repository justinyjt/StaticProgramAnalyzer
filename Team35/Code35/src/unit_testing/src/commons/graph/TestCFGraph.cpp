#include "catch.hpp"
#include "../../TestHelper.h"
#include "commons/graph/CFGraph.h"

TEST_CASE("1. test CFGraph get") {
    CFGraph graph = CFGraph();
    REQUIRE(graph.getNodes().empty());
    REQUIRE(graph.getEdges().empty());
    REQUIRE(graph.getNodes().size() == 0);
    REQUIRE(graph.getEdges().size() == 0);
}