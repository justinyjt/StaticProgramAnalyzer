#include "catch.hpp"
#include "../TestHelper.h"
#include "sp/CallGraph.h"

TEST_CASE("Call Graph can successfully add transitive caller-callee pairs") {
    SECTION("Single caller-callee add and retrieve") {
        CallGraph cg;
        ENT_ENT_SET expected;
        expected.emplace(ENT_ENT("a", "b"));
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
        expected.emplace(ENT_ENT("a", "b"));
        expected.emplace(ENT_ENT("a", "c"));
        expected.emplace(ENT_ENT("a", "d"));
        expected.emplace(ENT_ENT("b", "c"));
        expected.emplace(ENT_ENT("b", "d"));
        expected.emplace(ENT_ENT("c", "d"));
        cg.addCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
        cg.addCallRelationship(ENT_NAME("b"), ENT_NAME("c"));
        cg.addCallRelationship(ENT_NAME("c"), ENT_NAME("d"));
        requireEqual(expected, cg.getTransitiveCalls());
    }

    SECTION("Caller and callee correct direction") {
        CallGraph cg;
        ENT_ENT_SET expected;
        ENT_ENT_SET opposite_direction;
        expected.emplace(ENT_ENT("a", "b"));
        opposite_direction.emplace(ENT_ENT("b", "a"));
        cg.addCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
        requireEqual(expected, cg.getTransitiveCalls());
        requireNotEqual(opposite_direction, cg.getTransitiveCalls());
    }
}

TEST_CASE("Call Graph can successfully add immediate caller-callee pairs") {
    SECTION("Single caller-callee add and retrieve") {
        CallGraph cg;
        ENT_ENT_SET expected;
        expected.emplace(ENT_ENT("a", "b"));
        cg.addCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
        requireEqual(expected, cg.getImmediateCalls());
    }

    SECTION("Multiple caller-callee add and retrieve") {
        CallGraph cg;
        ENT_ENT_SET expected;
        expected.emplace(ENT_ENT("a", "b"));
        expected.emplace(ENT_ENT("b", "c"));
        expected.emplace(ENT_ENT("c", "d"));
        cg.addCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
        cg.addCallRelationship(ENT_NAME("b"), ENT_NAME("c"));
        cg.addCallRelationship(ENT_NAME("c"), ENT_NAME("d"));
        requireEqual(expected, cg.getImmediateCalls());
    }

    SECTION("Caller and callee correct direction") {
        CallGraph cg;
        ENT_ENT_SET expected;
        ENT_ENT_SET opposite_direction;
        expected.emplace(ENT_ENT("a", "b"));
        opposite_direction.emplace(ENT_ENT("b", "a"));
        cg.addCallRelationship(ENT_NAME("a"), ENT_NAME("b"));
        requireEqual(expected, cg.getTransitiveCalls());
        requireNotEqual(opposite_direction, cg.getTransitiveCalls());
    }
}