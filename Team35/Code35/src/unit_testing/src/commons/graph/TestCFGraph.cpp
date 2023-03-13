#include <cstdlib>

#include "catch.hpp"
#include "../../TestHelper.h"
#include "commons/graph/CFGraph.h"
#include "commons/graph/CFGraphNodeData.h"
#include "commons/types.h"

TEST_CASE("1. test CFGraph isReachable() method") {
    SECTION("1.1. empty graph") {
        CFG::CFGraph graph;
        requireFalse(graph.isReachable(1, 2));
    }

    SECTION("1.2. graph with one node") {
        CFG::CFGraph graph;
        graph.addNode(CFG::makeNodeData(1));
        requireFalse(graph.isReachable(1, 2));
    }

    SECTION("1.3. graph with two nodes") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        requireTrue(graph.isReachable(1, 2));
        requireFalse(graph.isReachable(2, 1));
    }

    SECTION("1.4. graph with two nodes and non-existent node") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        requireFalse(graph.isReachable(1, 3));
        requireFalse(graph.isReachable(4, 2));
    }

    SECTION("1.5. graph with two nodes and a loop") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(1));
        requireTrue(graph.isReachable(1, 2));
        requireTrue(graph.isReachable(2, 1));
    }

    SECTION("1.6. cycle graph, size 20") {
        CFG::CFGraph graph;
        for (int i = 1; i <= 20; i++) {
            graph.addNode(CFG::makeNodeData(i));
        }
        for (int i = 1; i <= 20; i++) {
            graph.addEdge(CFG::makeNodeData(i), CFG::makeNodeData(i % 20 + 1));
        }
        for (int i = 1; i <= 20; i++) {
            for (int j = 1; j <= 20; j++) {
                requireTrue(graph.isReachable(i, j));
            }
        }
    }

    SECTION("1.7. cycle graph, size 100") {
        CFG::CFGraph graph;
        for (int i = 1; i <= 100; i++) {
            graph.addNode(CFG::makeNodeData(i));
        }
        for (int i = 1; i <= 100; i++) {
            graph.addEdge(CFG::makeNodeData(i), CFG::makeNodeData(i % 100 + 1));
        }
        for (int i = 1; i <= 100; i++) {
            for (int j = 1; j <= 100; j++) {
                requireTrue(graph.isReachable(i, j));
            }
        }
    }

    SECTION("1.8. clique graph, size 20") {
        CFG::CFGraph graph;
        for (int i = 1; i <= 20; i++) {
            graph.addNode(CFG::makeNodeData(i));
        }
        for (int i = 1; i <= 20; i++) {
            for (int j = 1; j <= 20; j++) {
                if (i != j) {
                    graph.addEdge(CFG::makeNodeData(i), CFG::makeNodeData(j));
                }
            }
        }
        for (int i = 1; i <= 20; i++) {
            for (int j = 1; j <= 20; j++) {
                requireTrue(graph.isReachable(i, j));
            }
        }
    }

    SECTION("1.9. clique graph, size 100") {
        CFG::CFGraph graph;
        for (int i = 1; i <= 100; i++) {
            graph.addNode(CFG::makeNodeData(i));
        }
        for (int i = 1; i <= 100; i++) {
            for (int j = 1; j <= 100; j++) {
                if (i != j) {
                    graph.addEdge(CFG::makeNodeData(i), CFG::makeNodeData(j));
                }
            }
        }
        for (int i = 1; i <= 100; i++) {
            for (int j = 1; j <= 100; j++) {
                requireTrue(graph.isReachable(i, j));
            }
        }
    }

    SECTION("1.10. random graph, size 20") {
        CFG::CFGraph graph;
        for (int i = 1; i <= 20; i++) {
            graph.addNode(CFG::makeNodeData(i));
        }

        for (int i = 1; i <= 20; i++) {
            for (int j = 1; j <= 20; j++) {
                if (i != j && rand() % 2 == 0) {
                    graph.addEdge(CFG::makeNodeData(i), CFG::makeNodeData(j));
                }
            }
        }
        for (int i = 1; i <= 20; i++) {
            for (int j = 1; j <= 20; j++) {
                requireTrue(graph.isReachable(i, j));
            }
        }
    }

    SECTION("1.11. isReachable to itself via a loop") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(3));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(1));
        requireTrue(graph.isReachable(1, 1));
    }

    SECTION("1.12. line graph, size 20") {
        CFG::CFGraph graph;
        for (int i = 1; i <= 19; i++) {
            graph.addEdge(CFG::makeNodeData(i), CFG::makeNodeData(i + 1));
        }
        for (int i = 1; i <= 20; i++) {
            for (int j = 1; j <= 20; j++) {
                if (i < j) {
                    requireTrue(graph.isReachable(i, j));
                } else {
                    requireFalse(graph.isReachable(i, j));
                }
            }
        }
    }
}

TEST_CASE("2. test CFGraph getPredecessors() method, transitive closure") {
    SECTION("2.1. empty graph") {
        CFG::CFGraph graph;
        requireTrue(graph.getPredecessors(1, true).empty());
    }

    SECTION("2.2. graph with one node") {
        CFG::CFGraph graph;
        graph.addNode(CFG::makeNodeData(1));
        requireTrue(graph.getPredecessors(1, true).empty());
    }

    SECTION("2.3. graph with two nodes") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        requireTrue(graph.getPredecessors(1, true).empty());
        STMT_SET set = graph.getPredecessors(2, true);
        requireEqual(static_cast<int>(set.size()), 1);
        requireEqual(static_cast<int>(set.count(1)), 1);
    }

    SECTION("2.4. graph with two nodes and non-existent node") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        requireTrue(graph.getPredecessors(3, true).empty());
        requireTrue(graph.getPredecessors(4, true).empty());
    }

    SECTION("2.5. graph with two nodes and a loop") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(1));
        STMT_SET set1 = graph.getPredecessors(1, true);
        STMT_SET set2 = graph.getPredecessors(2, true);
        requireEqual(static_cast<int>(set1.size()), 2);
        requireEqual(static_cast<int>(set1.count(1)), 1);
        requireEqual(static_cast<int>(set1.count(2)), 1);
        requireEqual(static_cast<int>(set2.size()), 2);
        requireEqual(static_cast<int>(set2.count(1)), 1);
        requireEqual(static_cast<int>(set2.count(2)), 1);
    }

    SECTION("2.6. cycle graph, size 10") {
        CFG::CFGraph graph;
        for (int i = 1; i <= 10; i++) {
            graph.addEdge(CFG::makeNodeData(i), CFG::makeNodeData(i % 10 + 1));
        }
        for (int i = 1; i <= 10; i++) {
            STMT_SET set = graph.getPredecessors(i, true);
            requireEqual(static_cast<int>(set.size()), 10);
            for (int j = 1; j <= 10; j++) {
                requireEqual(static_cast<int>(set.count(j)), 1);
            }
        }
    }

    SECTION("2.7. tree graph, size 10") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(3));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(4));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(5));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(6));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(7));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeNodeData(8));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeNodeData(9));
        graph.addEdge(CFG::makeNodeData(5), CFG::makeNodeData(10));
        STMT_SET set1 = graph.getPredecessors(1, true);
        STMT_SET set6 = graph.getPredecessors(6, true);
        STMT_SET set8 = graph.getPredecessors(8, true);
        STMT_SET set10 = graph.getPredecessors(10, true);
        requireEqual(static_cast<int>(set1.size()), 0);
        requireEqual(static_cast<int>(set6.size()), 2);
        requireEqual(static_cast<int>(set6.count(1)), 1);
        requireEqual(static_cast<int>(set6.count(3)), 1);
        requireEqual(static_cast<int>(set8.size()), 3);
        requireEqual(static_cast<int>(set8.count(1)), 1);
        requireEqual(static_cast<int>(set8.count(2)), 1);
        requireEqual(static_cast<int>(set8.count(4)), 1);
        requireEqual(static_cast<int>(set10.size()), 3);
        requireEqual(static_cast<int>(set10.count(1)), 1);
        requireEqual(static_cast<int>(set10.count(2)), 1);
        requireEqual(static_cast<int>(set10.count(5)), 1);
    }

    SECTION("2.8. cycle graph with dummy nodes, should ignore dummy nodes, size 10") {
        CFG::CFGraph graph;
        // even nodes are dummy nodes
        for (int i = 1; i <= 10; i++) {
            if (i % 2) {
                graph.addEdge(CFG::makeNodeData(i), CFG::makeDummyNodeData(i % 10 + 1));
            } else {
                graph.addEdge(CFG::makeDummyNodeData(i), CFG::makeNodeData(i % 10 + 1));
            }
        }

        for (int i = 1; i <= 10; i++) {
            STMT_SET set = graph.getPredecessors(i, true);
            if (i % 2 == 0) {
                requireEqual(static_cast<int>(set.size()), 0);
                continue;
            }
            requireEqual(static_cast<int>(set.size()), 5);
            for (int j = 1; j <= 10; j++) {
                if (j % 2) {
                    requireEqual(static_cast<int>(set.count(j)), 1);
                }
            }
        }
    }

    SECTION("2.9. tree graph with dummy nodes, should ignore dummy nodes, size 10") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeDummyNodeData(2));
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(3));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(4));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(5));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeDummyNodeData(6));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(7));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeDummyNodeData(8));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeNodeData(9));
        graph.addEdge(CFG::makeNodeData(5), CFG::makeDummyNodeData(10));
        STMT_SET set1 = graph.getPredecessors(1, true);
        STMT_SET set6 = graph.getPredecessors(6, true);
        STMT_SET set8 = graph.getPredecessors(8, true);
        STMT_SET set9 = graph.getPredecessors(9, true);
        STMT_SET set10 = graph.getPredecessors(10, true);
        requireEqual(static_cast<int>(set1.size()), 0);
        requireEqual(static_cast<int>(set6.size()), 0);
        requireEqual(static_cast<int>(set8.size()), 0);
        requireEqual(static_cast<int>(set9.size()), 2);
        requireEqual(static_cast<int>(set9.count(1)), 1);
        requireEqual(static_cast<int>(set9.count(4)), 1);
        requireEqual(static_cast<int>(set10.size()), 0);
    }
}

TEST_CASE("3. test CFGraph getSuccessors() method, transitive closure") {
    SECTION("3.1. empty graph") {
        CFG::CFGraph graph;
        requireTrue(graph.getSuccessors(1, true).empty());
    }

    SECTION("3.2. graph with one node") {
        CFG::CFGraph graph;
        graph.addNode(CFG::makeNodeData(1));
        requireTrue(graph.getSuccessors(1, true).empty());
    }

    SECTION("3.3. graph with two nodes") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        STMT_SET set = graph.getSuccessors(1, true);
        requireEqual(static_cast<int>(set.size()), 1);
        requireEqual(static_cast<int>(set.count(2)), 1);
        requireTrue(graph.getSuccessors(2, true).empty());
    }

    SECTION("3.4. graph with two nodes and non-existent node") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        requireTrue(graph.getSuccessors(3, true).empty());
        requireTrue(graph.getSuccessors(4, true).empty());
    }

    SECTION("3.5. graph with two nodes and a loop") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(1));
        STMT_SET set1 = graph.getSuccessors(1, true);
        STMT_SET set2 = graph.getSuccessors(2, true);
        requireEqual(static_cast<int>(set1.size()), 2);
        requireEqual(static_cast<int>(set1.count(1)), 1);
        requireEqual(static_cast<int>(set1.count(2)), 1);
        requireEqual(static_cast<int>(set2.size()), 2);
        requireEqual(static_cast<int>(set2.count(1)), 1);
        requireEqual(static_cast<int>(set2.count(2)), 1);
    }

    SECTION("3.6. cycle graph, size 10") {
        CFG::CFGraph graph;
        for (int i = 1; i <= 10; i++) {
            graph.addEdge(CFG::makeNodeData(i), CFG::makeNodeData(i % 10 + 1));
        }
        for (int i = 1; i <= 10; i++) {
            STMT_SET set = graph.getSuccessors(i, true);
            requireEqual(static_cast<int>(set.size()), 10);
            for (int j = 1; j <= 10; j++) {
                requireEqual(static_cast<int>(set.count(j)), 1);
            }
        }
    }

    SECTION("3.7. tree graph, size 10") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(3));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(4));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(5));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(6));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(7));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeNodeData(8));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeNodeData(9));
        graph.addEdge(CFG::makeNodeData(5), CFG::makeNodeData(10));
        STMT_SET set1 = graph.getSuccessors(1, true);
        STMT_SET set2 = graph.getSuccessors(2, true);
        STMT_SET set6 = graph.getSuccessors(6, true);
        STMT_SET set8 = graph.getSuccessors(8, true);
        STMT_SET set10 = graph.getSuccessors(10, true);
        requireEqual(static_cast<int>(set1.size()), 9);
        requireEqual(static_cast<int>(set1.count(2)), 1);
        requireEqual(static_cast<int>(set1.count(3)), 1);
        requireEqual(static_cast<int>(set1.count(4)), 1);
        requireEqual(static_cast<int>(set1.count(5)), 1);
        requireEqual(static_cast<int>(set1.count(6)), 1);
        requireEqual(static_cast<int>(set1.count(7)), 1);
        requireEqual(static_cast<int>(set1.count(8)), 1);
        requireEqual(static_cast<int>(set1.count(9)), 1);
        requireEqual(static_cast<int>(set1.count(10)), 1);
        requireEqual(static_cast<int>(set2.size()), 5);
        requireEqual(static_cast<int>(set2.count(4)), 1);
        requireEqual(static_cast<int>(set2.count(5)), 1);
        requireEqual(static_cast<int>(set2.count(8)), 1);
        requireEqual(static_cast<int>(set2.count(9)), 1);
        requireEqual(static_cast<int>(set2.count(10)), 1);
        requireEqual(static_cast<int>(set6.size()), 0);
        requireEqual(static_cast<int>(set8.size()), 0);
        requireEqual(static_cast<int>(set10.size()), 0);
    }

    SECTION("3.8. cycle graph with dummy nodes, size 10") {
        CFG::CFGraph graph;
        // even nodes are dummy nodes
        for (int i = 1; i <= 10; i++) {
            if (i % 2) {
                graph.addEdge(CFG::makeNodeData(i), CFG::makeDummyNodeData(i % 10 + 1));
            } else {
                graph.addEdge(CFG::makeDummyNodeData(i), CFG::makeNodeData(i % 10 + 1));
            }
        }

        for (int i = 1; i <= 10; i++) {
            STMT_SET set = graph.getSuccessors(i, true);
            if (i % 2 == 0) {
                requireEqual(static_cast<int>(set.size()), 0);
                continue;
            }
            requireEqual(static_cast<int>(set.size()), 5);
            for (int j = 1; j <= 10; j++) {
                if (j % 2) {
                    requireEqual(static_cast<int>(set.count(j)), 1);
                }
            }
        }
    }

    SECTION("3.9. tree graph with dummy nodes, size 10") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeDummyNodeData(2));
        graph.addEdge(CFG::makeNodeData(1), CFG::makeDummyNodeData(3));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(4));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(5));
        graph.addEdge(CFG::makeDummyNodeData(3), CFG::makeNodeData(6));
        graph.addEdge(CFG::makeDummyNodeData(3), CFG::makeNodeData(7));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeDummyNodeData(4));
        graph.addEdge(CFG::makeNodeData(5), CFG::makeDummyNodeData(5));
        graph.addEdge(CFG::makeDummyNodeData(4), CFG::makeNodeData(8));
        graph.addEdge(CFG::makeDummyNodeData(4), CFG::makeNodeData(9));
        graph.addEdge(CFG::makeDummyNodeData(5), CFG::makeNodeData(10));
        STMT_SET set1 = graph.getSuccessors(1, true);
        STMT_SET set2 = graph.getSuccessors(2, true);
        STMT_SET set4 = graph.getSuccessors(4, true);
        STMT_SET set6 = graph.getSuccessors(6, true);
        STMT_SET set8 = graph.getSuccessors(8, true);
        STMT_SET set10 = graph.getSuccessors(10, true);
        requireEqual(static_cast<int>(set1.size()), 7);
        requireEqual(static_cast<int>(set1.count(4)), 1);
        requireEqual(static_cast<int>(set1.count(5)), 1);
        requireEqual(static_cast<int>(set1.count(6)), 1);
        requireEqual(static_cast<int>(set1.count(7)), 1);
        requireEqual(static_cast<int>(set1.count(8)), 1);
        requireEqual(static_cast<int>(set1.count(9)), 1);
        requireEqual(static_cast<int>(set1.count(10)), 1);
        requireEqual(static_cast<int>(set2.size()), 0);
        requireEqual(static_cast<int>(set4.size()), 2);
        requireEqual(static_cast<int>(set4.count(8)), 1);
        requireEqual(static_cast<int>(set4.count(9)), 1);
        requireEqual(static_cast<int>(set6.size()), 0);
        requireEqual(static_cast<int>(set8.size()), 0);
        requireEqual(static_cast<int>(set10.size()), 0);
    }
}

TEST_CASE("4. test CFGraph getPredecessors() method, non-transitive closure") {
    SECTION("4.1. empty graph") {
        CFG::CFGraph graph;
        requireTrue(graph.getPredecessors(1, false).empty());
    }

    SECTION("4.2. graph with one node") {
        CFG::CFGraph graph;
        graph.addNode(CFG::makeNodeData(1));
        requireTrue(graph.getPredecessors(1, false).empty());
    }

    SECTION("4.3. graph with two nodes") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        STMT_SET set = graph.getPredecessors(2, false);
        requireEqual(static_cast<int>(set.size()), 1);
        requireEqual(static_cast<int>(set.count(1)), 1);
        requireTrue(graph.getPredecessors(1, false).empty());
    }

    SECTION("4.4. graph with two nodes and non-existent node") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        requireTrue(graph.getPredecessors(3, false).empty());
        requireTrue(graph.getPredecessors(4, false).empty());
    }

    SECTION("4.5. graph with two nodes and a loop") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(1));
        STMT_SET set1 = graph.getPredecessors(1, false);
        STMT_SET set2 = graph.getPredecessors(2, false);
        requireEqual(static_cast<int>(set1.size()), 1);
        requireEqual(static_cast<int>(set1.count(2)), 1);
        requireEqual(static_cast<int>(set2.size()), 1);
        requireEqual(static_cast<int>(set2.count(1)), 1);
    }

    SECTION("4.6. cycle graph, size 10") {
        CFG::CFGraph graph;
        for (int i = 1; i <= 10; i++) {
            graph.addEdge(CFG::makeNodeData(i), CFG::makeNodeData(i % 10 + 1));
        }
        for (int i = 1; i <= 10; i++) {
            STMT_NUM j = (i % 10) + 1;
            STMT_SET set = graph.getPredecessors(j, false);
            requireEqual(static_cast<int>(set.size()), 1);
            requireEqual(static_cast<int>(set.count(i)), 1);
        }
    }

    SECTION("4.7. tree graph, size 10") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(3));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(4));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(5));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(6));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(7));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeNodeData(8));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeNodeData(9));
        graph.addEdge(CFG::makeNodeData(5), CFG::makeNodeData(10));
        STMT_SET set1 = graph.getPredecessors(1, false);
        STMT_SET set2 = graph.getPredecessors(2, false);
        STMT_SET set6 = graph.getPredecessors(6, false);
        STMT_SET set8 = graph.getPredecessors(8, false);
        STMT_SET set10 = graph.getPredecessors(10, false);
        requireEqual(static_cast<int>(set1.size()), 0);
        requireEqual(static_cast<int>(set2.size()), 1);
        requireEqual(static_cast<int>(set2.count(1)), 1);
        requireEqual(static_cast<int>(set6.size()), 1);
        requireEqual(static_cast<int>(set6.count(3)), 1);
        requireEqual(static_cast<int>(set8.size()), 1);
        requireEqual(static_cast<int>(set8.count(4)), 1);
        requireEqual(static_cast<int>(set10.size()), 1);
        requireEqual(static_cast<int>(set10.count(5)), 1);
    }

    SECTION("4.8. inverted chicken feet graph with middle dummy node") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeDummyNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeDummyNodeData(1));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeDummyNodeData(1));
        graph.addEdge(CFG::makeNodeData(5), CFG::makeDummyNodeData(1));
        STMT_SET set2 = graph.getPredecessors(2, false);
        requireEqual(static_cast<int>(set2.size()), 3);
        requireEqual(static_cast<int>(set2.count(3)), 1);
        requireEqual(static_cast<int>(set2.count(4)), 1);
        requireEqual(static_cast<int>(set2.count(5)), 1);
    }

    SECTION("4.8. inverted chicken feet graph with nested dummy node") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeDummyNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeDummyNodeData(3), CFG::makeDummyNodeData(1));
        graph.addEdge(CFG::makeNodeData(6), CFG::makeDummyNodeData(3));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeDummyNodeData(1));
        graph.addEdge(CFG::makeNodeData(5), CFG::makeDummyNodeData(1));
        STMT_SET set2 = graph.getPredecessors(2, false);
        requireEqual(static_cast<int>(set2.size()), 3);
        requireEqual(static_cast<int>(set2.count(6)), 1);
        requireEqual(static_cast<int>(set2.count(4)), 1);
        requireEqual(static_cast<int>(set2.count(5)), 1);
    }

    SECTION("4.9. cycle graph with dummy nodes") {
        CFG::CFGraph graph;
        // even nodes are dummy nodes
        for (int i = 1; i <= 10; i++) {
            if (i % 2) {
                graph.addEdge(CFG::makeNodeData(i), CFG::makeDummyNodeData(i % 10 + 1));
            } else {
                graph.addEdge(CFG::makeDummyNodeData(i), CFG::makeNodeData(i % 10 + 1));
            }
        }

        for (int i = 1; i <= 10; i++) {
            STMT_SET set = graph.getSuccessors(i, false);
            if (i % 2 == 0) {
                requireEqual(static_cast<int>(set.size()), 0);
                continue;
            }
            requireEqual(static_cast<int>(set.size()), 1);
            STMT_NUM j = ((i + 1) % 10) + 1;
            requireEqual(static_cast<int>(set.count(j)), 1);
        }
    }
}

TEST_CASE("5. test CFGraph getSuccessors() method, non-transitive closure") {
    SECTION("5.1. empty graph") {
        CFG::CFGraph graph;
        requireTrue(graph.getSuccessors(1, false).empty());
    }

    SECTION("5.2. graph with one node") {
        CFG::CFGraph graph;
        graph.addNode(CFG::makeNodeData(1));
        requireTrue(graph.getSuccessors(1, false).empty());
    }

    SECTION("5.3. graph with two nodes") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        STMT_SET set = graph.getSuccessors(1, false);
        requireEqual(static_cast<int>(set.size()), 1);
        requireEqual(static_cast<int>(set.count(2)), 1);
        requireTrue(graph.getSuccessors(2, false).empty());
    }

    SECTION("5.4. graph with two nodes and non-existent node") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        requireTrue(graph.getSuccessors(3, false).empty());
        requireTrue(graph.getSuccessors(4, false).empty());
    }

    SECTION("5.5. graph with two nodes and a loop") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(1));
        STMT_SET set1 = graph.getSuccessors(1, false);
        STMT_SET set2 = graph.getSuccessors(2, false);
        requireEqual(static_cast<int>(set1.size()), 1);
        requireEqual(static_cast<int>(set1.count(2)), 1);
        requireEqual(static_cast<int>(set2.size()), 1);
        requireEqual(static_cast<int>(set2.count(1)), 1);
    }

    SECTION("5.6. cycle graph, size 10") {
        CFG::CFGraph graph;
        for (int i = 1; i <= 10; i++) {
            graph.addEdge(CFG::makeNodeData(i), CFG::makeNodeData(i % 10 + 1));
        }
        for (int i = 1; i <= 10; i++) {
            STMT_NUM j = (i % 10) + 1;
            STMT_SET set = graph.getSuccessors(i, false);
            requireEqual(static_cast<int>(set.size()), 1);
            requireEqual(static_cast<int>(set.count(j)), 1);
        }
    }

    SECTION("5.7. chicken feet graph with middle dummy node") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeDummyNodeData(2));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(3));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(4));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(5));
        STMT_SET set1 = graph.getSuccessors(1, false);
        requireEqual(static_cast<int>(set1.size()), 3);
        requireEqual(static_cast<int>(set1.count(3)), 1);
        requireEqual(static_cast<int>(set1.count(4)), 1);
        requireEqual(static_cast<int>(set1.count(5)), 1);
        STMT_SET set2 = graph.getSuccessors(2, false);
        requireEqual(static_cast<int>(set2.size()), 0);
    }

    SECTION("5.8. chicken feet graph with nested dummy node") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeDummyNodeData(2));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeDummyNodeData(3));
        graph.addEdge(CFG::makeDummyNodeData(3), CFG::makeNodeData(6));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(4));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(5));
        STMT_SET set1 = graph.getSuccessors(1, false);
        requireEqual(static_cast<int>(set1.size()), 3);
        requireEqual(static_cast<int>(set1.count(6)), 1);
        requireEqual(static_cast<int>(set1.count(4)), 1);
        requireEqual(static_cast<int>(set1.count(5)), 1);
        STMT_SET set2 = graph.getSuccessors(2, false);
        requireEqual(static_cast<int>(set2.size()), 0);
    }

    SECTION("5.9. tree graph with dummy nodes") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeDummyNodeData(2));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(3));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(4));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(5));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeDummyNodeData(6));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeDummyNodeData(7));
        graph.addEdge(CFG::makeNodeData(5), CFG::makeDummyNodeData(8));
        graph.addEdge(CFG::makeDummyNodeData(6), CFG::makeNodeData(9));
        graph.addEdge(CFG::makeDummyNodeData(7), CFG::makeNodeData(10));
        graph.addEdge(CFG::makeDummyNodeData(8), CFG::makeNodeData(11));
        STMT_SET set1 = graph.getSuccessors(1, false);
        requireEqual(static_cast<int>(set1.size()), 3);
        requireEqual(static_cast<int>(set1.count(3)), 1);
        requireEqual(static_cast<int>(set1.count(4)), 1);
        requireEqual(static_cast<int>(set1.count(5)), 1);
        STMT_SET set2 = graph.getSuccessors(2, false);
        requireEqual(static_cast<int>(set2.size()), 0);
        STMT_SET set3 = graph.getSuccessors(3, false);
        requireEqual(static_cast<int>(set3.size()), 1);
        requireEqual(static_cast<int>(set3.count(9)), 1);
        STMT_SET set4 = graph.getSuccessors(4, false);
        requireEqual(static_cast<int>(set4.size()), 1);
        requireEqual(static_cast<int>(set4.count(10)), 1);
        STMT_SET set5 = graph.getSuccessors(5, false);
        requireEqual(static_cast<int>(set5.size()), 1);
        requireEqual(static_cast<int>(set5.count(11)), 1);
        STMT_SET set6 = graph.getSuccessors(6, false);
        requireEqual(static_cast<int>(set6.size()), 0);
        STMT_SET set7 = graph.getSuccessors(7, false);
        requireEqual(static_cast<int>(set7.size()), 0);
        STMT_SET set8 = graph.getSuccessors(8, false);
        requireEqual(static_cast<int>(set8.size()), 0);
    }
}

TEST_CASE("6. test getPairwiseControlFlow() method, transitive closure") {
    SECTION("6.1. empty graph") {
        CFG::CFGraph graph;
        STMT_STMT_SET result = graph.getPairwiseControlFlow(true);
        requireTrue(result.empty());
    }

    SECTION("6.2. graph with one node, self-loop") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(1));
        STMT_STMT_SET result = graph.getPairwiseControlFlow(true);
        requireEqual(static_cast<int>(result.size()), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 1))), 1);
    }

    SECTION("6.3. graph with two nodes") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        STMT_STMT_SET result = graph.getPairwiseControlFlow(true);
        requireEqual(static_cast<int>(result.size()), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 2))), 1);
    }

    SECTION("6.4. graph with two nodes and a loop") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(1));
        STMT_STMT_SET result = graph.getPairwiseControlFlow(true);
        requireEqual(static_cast<int>(result.size()), 4);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 1))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 2))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(2, 1))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(2, 2))), 1);
        // test memoization by test coverage
        STMT_STMT_SET result2 = graph.getPairwiseControlFlow(true);
        requireEqual(static_cast<int>(result2.size()), 4);
        requireEqual(result, result2);
    }

    SECTION("6.5. cycle graph") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(3));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(1));
        STMT_STMT_SET result = graph.getPairwiseControlFlow(true);
        requireEqual(static_cast<int>(result.size()), 9);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 1))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 2))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 3))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(2, 1))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(2, 2))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(2, 3))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(3, 1))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(3, 2))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(3, 3))), 1);
    }

    SECTION("6.6. tree graph, size 10") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(3));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(4));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(5));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(6));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(7));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeNodeData(8));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeNodeData(9));
        graph.addEdge(CFG::makeNodeData(5), CFG::makeNodeData(10));
        STMT_STMT_SET result = graph.getPairwiseControlFlow(true);
        requireEqual(static_cast<int>(result.size()), 19);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 2))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 3))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 4))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 5))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 6))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 7))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 8))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 9))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 10))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(2, 4))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(2, 5))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(2, 8))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(2, 9))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(2, 10))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(3, 6))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(3, 7))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(4, 8))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(4, 9))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(5, 10))), 1);
    }

    SECTION("6.7. inverted chicken feet graph with middle dummy node, nested") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeDummyNodeData(2));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeDummyNodeData(3));
        graph.addEdge(CFG::makeDummyNodeData(3), CFG::makeNodeData(6));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(4));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(5));
        STMT_STMT_SET result = graph.getPairwiseControlFlow(true);
        requireEqual(static_cast<int>(result.size()), 3);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 4))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 5))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 6))), 1);
    }
}

TEST_CASE("6. test getPairwiseControlFlow() method, non-transitive closure") {
    SECTION("6.1. empty graph") {
        CFG::CFGraph graph;
        STMT_STMT_SET result = graph.getPairwiseControlFlow(false);
        requireTrue(result.empty());
    }

    SECTION("6.2. graph with one node, self-loop") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(1));
        STMT_STMT_SET result = graph.getPairwiseControlFlow(false);
        requireEqual(static_cast<int>(result.size()), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 1))), 1);
    }

    SECTION("6.3. graph with two nodes") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        STMT_STMT_SET result = graph.getPairwiseControlFlow(false);
        requireEqual(static_cast<int>(result.size()), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 2))), 1);
    }

    SECTION("6.4. graph with two nodes and a loop") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(1));
        STMT_STMT_SET result = graph.getPairwiseControlFlow(false);
        requireEqual(static_cast<int>(result.size()), 2);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 2))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(2, 1))), 1);
        // test memoization by test coverage
        STMT_STMT_SET result2 = graph.getPairwiseControlFlow(false);
        requireEqual(static_cast<int>(result2.size()), 2);
        requireEqual(result, result2);
    }

    SECTION("6.5. cycle graph") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(3));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(1));
        STMT_STMT_SET result = graph.getPairwiseControlFlow(false);
        requireEqual(static_cast<int>(result.size()), 3);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 2))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(2, 3))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(3, 1))), 1);
    }

    SECTION("6.6. tree graph, size 10") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(3));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(4));
        graph.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(5));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(6));
        graph.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(7));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeNodeData(8));
        graph.addEdge(CFG::makeNodeData(4), CFG::makeNodeData(9));
        graph.addEdge(CFG::makeNodeData(5), CFG::makeNodeData(10));
        STMT_STMT_SET result = graph.getPairwiseControlFlow(false);
        requireEqual(static_cast<int>(result.size()), 9);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 2))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 3))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(2, 4))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(2, 5))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(3, 6))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(3, 7))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(4, 8))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(4, 9))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(5, 10))), 1);
    }

    SECTION("6.7. inverted chicken feet graph with middle dummy node, nested") {
        CFG::CFGraph graph;
        graph.addEdge(CFG::makeNodeData(1), CFG::makeDummyNodeData(2));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeDummyNodeData(3));
        graph.addEdge(CFG::makeDummyNodeData(3), CFG::makeNodeData(6));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(4));
        graph.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(5));
        STMT_STMT_SET result = graph.getPairwiseControlFlow(false);
        requireEqual(static_cast<int>(result.size()), 3);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 4))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 5))), 1);
        requireEqual(static_cast<int>(result.count(std::make_pair(1, 6))), 1);
    }
}

TEST_CASE("7. test == / != operator") {
    SECTION("7.1. empty graphs") {
        CFG::CFGraph graph1;
        CFG::CFGraph graph2;
        requireTrue(graph1 == graph2);
        requireFalse(graph1 != graph2);
    }

    SECTION("7.2. graph with one node, self-loop") {
        CFG::CFGraph graph1;
        graph1.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(1));
        CFG::CFGraph graph2;
        graph2.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(1));
        requireTrue(graph1 == graph2);
        requireFalse(graph1 != graph2);
    }

    SECTION("7.3. graph with two nodes") {
        CFG::CFGraph graph1;
        graph1.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        CFG::CFGraph graph2;
        graph2.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        requireTrue(graph1 == graph2);
        requireFalse(graph1 != graph2);
    }

    SECTION("7.4. graph with two nodes and a loop") {
        CFG::CFGraph graph1;
        graph1.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph1.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(1));
        CFG::CFGraph graph2;
        graph2.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph2.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(1));
        requireTrue(graph1 == graph2);
        requireFalse(graph1 != graph2);
    }

    SECTION("7.5. cycle graph") {
        CFG::CFGraph graph1;
        graph1.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph1.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(3));
        graph1.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(1));
        CFG::CFGraph graph2;
        graph2.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph2.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(3));
        graph2.addEdge(CFG::makeNodeData(3), CFG::makeNodeData(1));
        requireTrue(graph1 == graph2);
        requireFalse(graph1 != graph2);
    }

    SECTION("7.6. tree graph, size 3") {
        CFG::CFGraph graph1;
        graph1.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph1.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(3));
        CFG::CFGraph graph2;
        graph2.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph2.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(3));
        requireTrue(graph1 == graph2);
        requireFalse(graph1 != graph2);
    }

    SECTION("7.7. tree graph with other data members") {
        CFG::CFGraph graph1(CFG::CFGraph(), 1, 2, "main");
        graph1.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph1.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(3));
        CFG::CFGraph graph2(CFG::CFGraph(), 1, 2, "main");
        graph2.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph2.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(3));
        requireTrue(graph1 == graph2);
        requireFalse(graph1 != graph2);
    }

    SECTION("7.8. tree graph with other data members - not same") {
        CFG::CFGraph graph1(CFG::CFGraph(), 1, 2, "main");
        graph1.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        CFG::CFGraph graph2(CFG::CFGraph(), 1, 2, "main");
        graph2.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        graph2.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(3));
        requireTrue(graph1 != graph2);
        requireFalse(graph1 == graph2);
    }
}
