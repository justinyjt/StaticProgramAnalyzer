#include "catch.hpp"
#include "../TestHelper.h"
#include "sp/CallGraph.h"

TEST_CASE("Call Graph can successfully add caller-callee pairs") {
    SECTION("Single caller-callee add and retrieve") {
        CallGraph cg;
        ENT_ENT_SET expected;
        expected.emplace(ENT_ENT("a", "b"));
        cg.addTransitiveCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
        requireEqual(expected, cg.getTransitiveCalls());
    }

        /**
         * a->b->c->d
         * b->c->d
         * c->d
         */
    SECTION("Cascading caller-callee add and retrieve") {
        CallGraph cg;
        ENT_ENT_SET expected;
        expected.emplace(ENT_ENT("a", "b"));
        expected.emplace(ENT_ENT("a", "c"));
        expected.emplace(ENT_ENT("a", "d"));
        expected.emplace(ENT_ENT("b", "c"));
        expected.emplace(ENT_ENT("b", "d"));
        expected.emplace(ENT_ENT("c", "d"));
        cg.addTransitiveCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
        cg.addTransitiveCallRelationship(ENT_NAME("a"), ENT_NAME("c"));
        cg.addTransitiveCallRelationship(ENT_NAME("a"), ENT_NAME("d"));
        cg.addTransitiveCallRelationship(ENT_NAME("b"), ENT_NAME("c"));
        cg.addTransitiveCallRelationship(ENT_NAME("b"), ENT_NAME("d"));
        cg.addTransitiveCallRelationship(ENT_NAME("c"), ENT_NAME("d"));
        requireEqual(expected, cg.getTransitiveCalls());
    }

    SECTION("Caller and callee correct direction") {
        CallGraph cg;
        ENT_ENT_SET expected;
        ENT_ENT_SET opposite_direction;
        expected.emplace(ENT_ENT("a", "b"));
        opposite_direction.emplace(ENT_ENT("b", "a"));
        cg.addTransitiveCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
        requireEqual(expected, cg.getTransitiveCalls());
        requireNotEqual(opposite_direction, cg.getTransitiveCalls());
    }
}