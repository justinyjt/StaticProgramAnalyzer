#include "catch.hpp"
#include "../../TestHelper.h"
#include "pkb/db/AffectsGraph.h"

TEST_CASE("1. test hasAffectsRelationship() method") {
    SECTION("1.1. basic retrieval") {
        AffectsGraph graph;
        graph.addAffectsEdge(1, 2);
        graph.addAffectsEdge(2, 3);
        requireTrue(graph.hasAffectsRelationship(1, 2));
        requireTrue(graph.hasAffectsRelationship(2, 3));
        requireFalse(graph.hasAffectsRelationship(2, 1));
        requireFalse(graph.hasAffectsRelationship(1, 3));
        requireFalse(graph.hasAffectsRelationship(3, 1));
        requireFalse(graph.hasAffectsRelationship(3, 2));
    }

    SECTION("1.2. basic retrieval with duplicated edges") {
        AffectsGraph graph;
        graph.addAffectsEdge(1, 2);
        graph.addAffectsEdge(2, 3);
        graph.addAffectsEdge(1, 2);
        requireTrue(graph.hasAffectsRelationship(1, 2));
        requireTrue(graph.hasAffectsRelationship(2, 3));
        requireFalse(graph.hasAffectsRelationship(2, 1));
        requireFalse(graph.hasAffectsRelationship(1, 3));
        requireFalse(graph.hasAffectsRelationship(3, 1));
        requireFalse(graph.hasAffectsRelationship(3, 2));
    }
}

TEST_CASE("2. test getAllAffectsRelationships() method") {
    SECTION("2.1. basic retrieval of non-transitive relationships") {
        AffectsGraph graph;
        graph.addAffectsEdge(1, 2);
        graph.addAffectsEdge(2, 3);
        graph.addAffectsEdge(3, 4);
        graph.addAffectsEdge(4, 5);
        STMT_STMT_SET expected;
        expected.emplace(1, 2);
        expected.emplace(2, 3);
        expected.emplace(3, 4);
        expected.emplace(4, 5);
        requireEqualRef(graph.getAllAffectsRelationships(false), expected);
        requireEqualRef(graph.getAllAffectsRelationships(false), expected);
    }

    SECTION("2.2. basic retrieval of transitive relationships") {
        AffectsGraph graph;
        graph.addAffectsEdge(1, 2);
        graph.addAffectsEdge(2, 3);
        graph.addAffectsEdge(3, 4);
        graph.addAffectsEdge(4, 5);
        STMT_STMT_SET expected;
        expected.emplace(1, 2);
        expected.emplace(1, 3);
        expected.emplace(1, 4);
        expected.emplace(1, 5);
        expected.emplace(2, 3);
        expected.emplace(2, 4);
        expected.emplace(2, 5);
        expected.emplace(3, 4);
        expected.emplace(3, 5);
        expected.emplace(4, 5);
        requireEqualRef(graph.getAllAffectsRelationships(true), expected);
        requireEqualRef(graph.getAllAffectsRelationships(true), expected);
    }

    SECTION("2.3. retrieval of transitive relationships in cyclic graph") {
        AffectsGraph graph;
        graph.addAffectsEdge(1, 2);
        graph.addAffectsEdge(2, 3);
        graph.addAffectsEdge(3, 4);
        graph.addAffectsEdge(4, 5);
        graph.addAffectsEdge(5, 1);
        STMT_STMT_SET expected;
        expected.emplace(1, 1);
        expected.emplace(1, 2);
        expected.emplace(1, 3);
        expected.emplace(1, 4);
        expected.emplace(1, 5);
        expected.emplace(2, 1);
        expected.emplace(2, 2);
        expected.emplace(2, 3);
        expected.emplace(2, 4);
        expected.emplace(2, 5);
        expected.emplace(3, 1);
        expected.emplace(3, 2);
        expected.emplace(3, 3);
        expected.emplace(3, 4);
        expected.emplace(3, 5);
        expected.emplace(4, 1);
        expected.emplace(4, 2);
        expected.emplace(4, 3);
        expected.emplace(4, 4);
        expected.emplace(4, 5);
        expected.emplace(5, 1);
        expected.emplace(5, 2);
        expected.emplace(5, 3);
        expected.emplace(5, 4);
        expected.emplace(5, 5);
        requireEqualRef(graph.getAllAffectsRelationships(true), expected);
        requireEqualRef(graph.getAllAffectsRelationships(true), expected);
    }
}
