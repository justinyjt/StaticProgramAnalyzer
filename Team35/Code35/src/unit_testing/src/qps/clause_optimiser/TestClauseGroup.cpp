#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/clause_optimiser/ClauseGroup.h"

TEST_CASE("1. test == / != operator") {
    SECTION("1.1. equal") {
        ClauseGroup clause_group_1({0, 1, 2}, {"0", "1", "2"});
        ClauseGroup clause_group_2({0, 1, 2}, {"0", "1", "2"});
        requireTrue(clause_group_1 == clause_group_2);
    }

    SECTION("1.2. not equal - different clause join order") {
        ClauseGroup clause_group_1({0, 1, 2}, {"0", "1", "2"});
        ClauseGroup clause_group_2({0, 1, 3}, {"0", "1", "2"});
        requireFalse(clause_group_1 == clause_group_2);
        requireTrue(clause_group_1 != clause_group_2);
    }

    SECTION("1.3. not equal - different group headers") {
        ClauseGroup clause_group_1({0, 1, 2}, {"0", "1", "2"});
        ClauseGroup clause_group_2({0, 1, 2}, {"0", "1", "3"});
        requireFalse(clause_group_1 == clause_group_2);
        requireTrue(clause_group_1 != clause_group_2);
    }
}
