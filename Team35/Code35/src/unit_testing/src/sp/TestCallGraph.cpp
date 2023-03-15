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
        cg.addProcScope("a", STMT_STMT(1, 3));
        cg.addProcScope("b", STMT_STMT(4, 6));
        cg.addProcScope("c", STMT_STMT(7, 7));
        cg.addProcScope("d", STMT_STMT(8, 9));
        STMT_SET a = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        STMT_SET b = {4, 5, 6, 7, 8, 9};
        STMT_SET c = {7, 8, 9};
        STMT_SET d = {8, 9};
        requireEqual(a, cg.getStmts("a"));
        requireEqual(b, cg.getStmts("b"));
        requireEqual(c, cg.getStmts("c"));
        requireEqual(d, cg.getStmts("d"));
    };
}
