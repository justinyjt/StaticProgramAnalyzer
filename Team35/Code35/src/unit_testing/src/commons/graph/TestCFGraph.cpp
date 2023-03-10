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
        requireEqual((int) set.size(), 1);
        requireEqual((int) set.count(1), 1);
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
        requireEqual((int) set1.size(), 2);
        requireEqual((int) set1.count(1), 1);
        requireEqual((int) set1.count(2), 1);
        requireEqual((int) set2.size(), 2);
        requireEqual((int) set2.count(1), 1);
        requireEqual((int) set2.count(2), 1);
    }

    SECTION("2.6. cycle graph, size 10") {
        CFG::CFGraph graph;
        for (int i = 1; i <= 10; i++) {
            graph.addEdge(CFG::makeNodeData(i), CFG::makeNodeData(i % 10 + 1));
        }
        for (int i = 1; i <= 10; i++) {
            STMT_SET set = graph.getPredecessors(i, true);
            requireEqual((int) set.size(), 10);
            for (int j = 1; j <= 10; j++) {
                requireEqual((int) set.count(j), 1);
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
        requireEqual((int) set1.size(), 0);
        requireEqual((int) set6.size(), 2);
        requireEqual((int) set6.count(1), 1);
        requireEqual((int) set6.count(3), 1);
        requireEqual((int) set8.size(), 3);
        requireEqual((int) set8.count(1), 1);
        requireEqual((int) set8.count(2), 1);
        requireEqual((int) set8.count(4), 1);
        requireEqual((int) set10.size(), 3);
        requireEqual((int) set10.count(1), 1);
        requireEqual((int) set10.count(2), 1);
        requireEqual((int) set10.count(5), 1);
    }
    // TODO(ngjunkang): test dummy node
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
        requireEqual((int) set.size(), 1);
        requireEqual((int) set.count(2), 1);
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
        requireEqual((int) set1.size(), 2);
        requireEqual((int) set1.count(1), 1);
        requireEqual((int) set1.count(2), 1);
        requireEqual((int) set2.size(), 2);
        requireEqual((int) set2.count(1), 1);
        requireEqual((int) set2.count(2), 1);
    }

    SECTION("3.6. cycle graph, size 10") {
        CFG::CFGraph graph;
        for (int i = 1; i <= 10; i++) {
            graph.addEdge(CFG::makeNodeData(i), CFG::makeNodeData(i % 10 + 1));
        }
        for (int i = 1; i <= 10; i++) {
            STMT_SET set = graph.getSuccessors(i, true);
            requireEqual((int) set.size(), 10);
            for (int j = 1; j <= 10; j++) {
                requireEqual((int) set.count(j), 1);
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
        requireEqual((int) set1.size(), 9);
        requireEqual((int) set1.count(2), 1);
        requireEqual((int) set1.count(3), 1);
        requireEqual((int) set1.count(4), 1);
        requireEqual((int) set1.count(5), 1);
        requireEqual((int) set1.count(6), 1);
        requireEqual((int) set1.count(7), 1);
        requireEqual((int) set1.count(8), 1);
        requireEqual((int) set1.count(9), 1);
        requireEqual((int) set1.count(10), 1);
        requireEqual((int) set2.size(), 5);
        requireEqual((int) set2.count(4), 1);
        requireEqual((int) set2.count(5), 1);
        requireEqual((int) set2.count(8), 1);
        requireEqual((int) set2.count(9), 1);
        requireEqual((int) set2.count(10), 1);
        requireEqual((int) set6.size(), 0);
        requireEqual((int) set8.size(), 0);
        requireEqual((int) set10.size(), 0);
    }
    // TODO(ngjunkang): test dummy node
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
        requireEqual((int) set.size(), 1);
        requireEqual((int) set.count(1), 1);
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
        requireEqual((int) set1.size(), 1);
        requireEqual((int) set1.count(2), 1);
        requireEqual((int) set2.size(), 1);
        requireEqual((int) set2.count(1), 1);
    }

    SECTION("4.6. cycle graph, size 10") {
        CFG::CFGraph graph;
        for (int i = 1; i <= 10; i++) {
            graph.addEdge(CFG::makeNodeData(i), CFG::makeNodeData(i % 10 + 1));
        }
        for (int i = 1; i <= 10; i++) {
            STMT_NUM j = (i % 10) + 1;
            STMT_SET set = graph.getPredecessors(j, false);
            requireEqual((int) set.size(), 1);
            requireEqual((int) set.count(i), 1);
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
        requireEqual((int) set1.size(), 0);
        requireEqual((int) set2.size(), 1);
        requireEqual((int) set2.count(1), 1);
        requireEqual((int) set6.size(), 1);
        requireEqual((int) set6.count(3), 1);
        requireEqual((int) set8.size(), 1);
        requireEqual((int) set8.count(4), 1);
        requireEqual((int) set10.size(), 1);
        requireEqual((int) set10.count(5), 1);
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
        requireEqual((int) set.size(), 1);
        requireEqual((int) set.count(2), 1);
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
        requireEqual((int) set1.size(), 1);
        requireEqual((int) set1.count(2), 1);
        requireEqual((int) set2.size(), 1);
        requireEqual((int) set2.count(1), 1);
    }

    SECTION("5.6. cycle graph, size 10") {
        CFG::CFGraph graph;
        for (int i = 1; i <= 10; i++) {
            graph.addEdge(CFG::makeNodeData(i), CFG::makeNodeData(i % 10 + 1));
        }
        for (int i = 1; i <= 10; i++) {
            STMT_NUM j = (i % 10) + 1;
            STMT_SET set = graph.getSuccessors(i, false);
            requireEqual((int) set.size(), 1);
            requireEqual((int) set.count(j), 1);
        }
    }
}
