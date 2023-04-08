#include "catch.hpp"
#include "../TestHelper.h"
#include "commons/graph/CallGraph.h"

TEST_CASE("Call Graph can successfully add transitive caller-callee pairs") {
    SECTION("Single caller-callee add and retrieve") {
        CallGraph cg;
        ENT_ENT_SET expected;
        expected.emplace("a", "b");
        cg.addCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
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
        expected.emplace("a", "b");
        expected.emplace("a", "c");
        expected.emplace("a", "d");
        expected.emplace("b", "c");
        expected.emplace("b", "d");
        expected.emplace("c", "d");
        cg.addCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
        cg.addCallRelationship(ENT_NAME("b"), ENT_NAME("c"));
        cg.addCallRelationship(ENT_NAME("c"), ENT_NAME("d"));
        requireEqual(expected, cg.getTransitiveCalls());
    }

    SECTION("Caller and callee correct direction") {
        CallGraph cg;
        ENT_ENT_SET expected;
        ENT_ENT_SET opposite_direction;
        expected.emplace("a", "b");
        opposite_direction.emplace("b", "a");
        cg.addCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
        requireEqual(expected, cg.getTransitiveCalls());
        requireNotEqual(opposite_direction, cg.getTransitiveCalls());
    }
}

TEST_CASE("Call Graph can successfully add immediate caller-callee pairs") {
    SECTION("Single caller-callee add and retrieve") {
        CallGraph cg;
        ENT_ENT_SET expected;
        expected.emplace("a", "b");
        cg.addCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
        requireEqual(expected, cg.getImmediateCalls());
    }

    SECTION("Multiple caller-callee add and retrieve") {
        CallGraph cg;
        ENT_ENT_SET expected;
        expected.emplace("a", "b");
        expected.emplace("b", "c");
        expected.emplace("c", "d");
        cg.addCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
        cg.addCallRelationship(ENT_NAME("b"), ENT_NAME("c"));
        cg.addCallRelationship(ENT_NAME("c"), ENT_NAME("d"));
        requireEqual(expected, cg.getImmediateCalls());
    }

    SECTION("Caller and callee correct direction") {
        CallGraph cg;
        ENT_ENT_SET expected;
        ENT_ENT_SET opposite_direction;
        expected.emplace("a", "b");
        opposite_direction.emplace("b", "a");
        cg.addCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
        requireEqual(expected, cg.getTransitiveCalls());
        requireNotEqual(opposite_direction, cg.getTransitiveCalls());
    }
}

TEST_CASE("Call Graph can successfully add graphs with stmt_num scope") {
    SECTION("standard add and retrieve") {
        CallGraph cg;
        ENT_ENT_SET expected;
        expected.emplace("a", "b");
        expected.emplace("a", "c");
        expected.emplace("a", "d");
        expected.emplace("b", "c");
        expected.emplace("b", "d");
        expected.emplace("c", "d");
        cg.addCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
        cg.addCallRelationship(ENT_NAME("b"), ENT_NAME("c"));
        cg.addCallRelationship(ENT_NAME("c"), ENT_NAME("d"));
        requireEqual(expected, cg.getTransitiveCalls());
    };
}
