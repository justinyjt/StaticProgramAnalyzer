#include "commons/TNode.h"
#include "catch.hpp"

void require(bool b) {
    REQUIRE(b);
}

TEST_CASE("1st Test") {
    TNode T;
    require(1 == 1);
}

