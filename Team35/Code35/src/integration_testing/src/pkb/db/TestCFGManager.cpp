#include "catch.hpp"
#include "../../TestHelper.h"
#include "pkb/db/CFGManager.h"
#include "sp/CFGraphBuilder.h"
#include "commons/graph/CFGraph.h"

TEST_CASE("1. CFG with one line") {
    CFG::CFGManager manager;
    CFG::CFGraphBuilder cf_graph_builder;
    cf_graph_builder.addStmt(1);
    cf_graph_builder.setMinStmtNum(1);
    cf_graph_builder.setMaxStmtNum(1);
    cf_graph_builder.setProcName("proc");
    CFG::CFGraph cf_graph = cf_graph_builder.build();
    std::vector<CFG::CFGraph> test;
    test.emplace_back(std::move(cf_graph));
    manager.setGraphs(std::move(test));
    requireFalse(manager.isNextExists());

}

TEST_CASE("2. Test isNextExists in CFGManager with valid CFG") {
    CFG::CFGManager manager;
    CFG::CFGraphBuilder cf_graph_builder;
    cf_graph_builder.addStmt(1);
    cf_graph_builder.addStmt(2);
    cf_graph_builder.addDummyNode(1);
    cf_graph_builder.setLastVisitedStmt(1);
    cf_graph_builder.addStmt(3);
    cf_graph_builder.addDummyNode(1);
    cf_graph_builder.addStmt(4);
    cf_graph_builder.addStmt(5);
    cf_graph_builder.addDummyNode(4);
    cf_graph_builder.setLastVisitedStmt(4);
    cf_graph_builder.addStmt(6);
    cf_graph_builder.addDummyNode(4);
    cf_graph_builder.setMaxStmtNum(6);
    cf_graph_builder.setMinStmtNum(1);
    cf_graph_builder.setProcName("proc");
    CFG::CFGraph cf_graph = cf_graph_builder.build();
    std::vector<CFG::CFGraph> test;
    test.emplace_back(std::move(cf_graph));
    manager.setGraphs(std::move(test));
    SECTION("Test is next exist") {
        requireTrue(manager.isNextExists());
    }SECTION("Test get connected statements") {
        {
            STMT_SET expected;
            expected.emplace(2);
            expected.emplace(3);
            requireEqual(expected, manager.getConnectedStmts(1, true, false));
        }
        {
            STMT_SET expected;
            expected.emplace(5);
            expected.emplace(6);
            requireEqual(expected, manager.getConnectedStmts(4, true, false));
        }
        {
            STMT_SET expected;
            expected.emplace(4);
            requireEqual(expected, manager.getConnectedStmts(2, true, false));
            requireEqual(expected, manager.getConnectedStmts(3, true, false));
        }
        {
            STMT_SET expected;
            requireEqual(expected, manager.getConnectedStmts(5, true, false));
            requireEqual(expected, manager.getConnectedStmts(6, true, false));
        }
        requireFalse(manager.isNextExistAfterStmtNum(5, true));
        requireFalse(manager.isNextExistAfterStmtNum(6, false));
    }

    SECTION("Test is next exist by key") {
        requireTrue(manager.isNextExistAfterStmtNum(1, true));
        requireTrue(manager.isNextExistAfterStmtNum(2, true));
        requireTrue(manager.isNextExistAfterStmtNum(3, true));
        requireTrue(manager.isNextExistAfterStmtNum(4, true));
        requireFalse(manager.isNextExistAfterStmtNum(5, true));
        requireFalse(manager.isNextExistAfterStmtNum(6, false));

    }

    SECTION("Test is next exist by val") {
        requireTrue(manager.isNextExistBeforeStmtNum(6, true));
        requireTrue(manager.isNextExistBeforeStmtNum(5, true));
        requireTrue(manager.isNextExistBeforeStmtNum(4, true));
        requireTrue(manager.isNextExistBeforeStmtNum(3, true));
        requireTrue(manager.isNextExistBeforeStmtNum(2, true));
        requireFalse(manager.isNextExistBeforeStmtNum(1, true));
        requireTrue(manager.isNextExistBeforeStmtNum(6, false));
        requireTrue(manager.isNextExistBeforeStmtNum(5, false));
        requireTrue(manager.isNextExistBeforeStmtNum(4, false));
        requireTrue(manager.isNextExistBeforeStmtNum(3, false));
        requireTrue(manager.isNextExistBeforeStmtNum(2, false));
        requireFalse(manager.isNextExistBeforeStmtNum(1, false));
    }
}

TEST_CASE("3. test isNext() method") {
    /**
     * if, stmt, while loop, stmt
     */
    CFG::CFGraphBuilder cf_graph_builder;
    cf_graph_builder.addStmt(1);
    cf_graph_builder.addStmt(2);
    cf_graph_builder.addStmt(3);
    cf_graph_builder.addDummyNode(1);
    cf_graph_builder.setLastVisitedStmt(1);
    cf_graph_builder.addStmt(4);
    cf_graph_builder.addStmt(5);
    cf_graph_builder.addDummyNode(1);
    cf_graph_builder.addStmt(6);  // stmt
    cf_graph_builder.addStmt(7);  // while loop
    cf_graph_builder.addStmt(8);
    cf_graph_builder.addStmt(9);
    cf_graph_builder.addLoop(7);
    cf_graph_builder.addStmt(10);

    cf_graph_builder.setMaxStmtNum(10);
    cf_graph_builder.setMinStmtNum(1);
    cf_graph_builder.setProcName("proc");
    CFG::CFGraph cf_graph = cf_graph_builder.build();
    std::vector<CFG::CFGraph> test;
    test.emplace_back(std::move(cf_graph));
    CFG::CFGManager manager;
    manager.setGraphs(std::move(test));

    SECTION("3.1. all possible isNext() non-transitive") {
        requireTrue(manager.isNext(1, 2, false));
        requireFalse(manager.isNext(1, 3, false));
        requireTrue(manager.isNext(1, 4, false));
        requireFalse(manager.isNext(1, 5, false));
        requireFalse(manager.isNext(1, 6, false));
        requireFalse(manager.isNext(1, 7, false));
        requireFalse(manager.isNext(1, 8, false));
        requireFalse(manager.isNext(1, 9, false));
        requireFalse(manager.isNext(1, 10, false));
        requireTrue(manager.isNext(2, 3, false));
        requireFalse(manager.isNext(2, 4, false));
        requireFalse(manager.isNext(2, 5, false));
        requireFalse(manager.isNext(2, 6, false));
        requireFalse(manager.isNext(2, 7, false));
        requireFalse(manager.isNext(2, 8, false));
        requireFalse(manager.isNext(2, 9, false));
        requireFalse(manager.isNext(2, 10, false));
        requireFalse(manager.isNext(3, 4, false));
        requireFalse(manager.isNext(3, 5, false));
        requireTrue(manager.isNext(3, 6, false));
        requireFalse(manager.isNext(3, 7, false));
        requireFalse(manager.isNext(3, 8, false));
        requireFalse(manager.isNext(3, 9, false));
        requireFalse(manager.isNext(3, 10, false));
        requireTrue(manager.isNext(4, 5, false));
        requireFalse(manager.isNext(4, 6, false));
        requireFalse(manager.isNext(4, 7, false));
        requireFalse(manager.isNext(4, 8, false));
        requireFalse(manager.isNext(4, 9, false));
        requireFalse(manager.isNext(4, 10, false));
        requireTrue(manager.isNext(5, 6, false));
        requireTrue(manager.isNext(7, 8, false));
        requireTrue(manager.isNext(8, 9, false));
        requireTrue(manager.isNext(9, 7, false));
        requireTrue(manager.isNext(7, 10, false));
    }
}

TEST_CASE("4. getValidNextPairs, getValidPredecessors, getValidSuccessors") {
    /**
     * if, stmt, while loop, stmt
     */
    CFG::CFGraphBuilder cf_graph_builder;
    std::vector<CFG::CFGraph> test;
    STMT_STMT_SET expected;
    STMT_STMT_SET expectedT;
    cf_graph_builder.addStmt(1);
    cf_graph_builder.addStmt(2);
    cf_graph_builder.addStmt(3);
    cf_graph_builder.addLoop(1);
    cf_graph_builder.setMaxStmtNum(3);
    cf_graph_builder.setMinStmtNum(1);
    cf_graph_builder.setProcName("proc");
    CFG::CFGraph cf_graph = cf_graph_builder.build();
    {
        STMT_STMT_SET allPairs = cf_graph.getPairwiseControlFlow(false);
        expected.insert(allPairs.begin(), allPairs.end());
        STMT_STMT_SET allPairsT = cf_graph.getPairwiseControlFlow(true);
        expectedT.insert(allPairsT.begin(), allPairsT.end());
    }
    test.emplace_back(std::move(cf_graph));
    cf_graph_builder.reset();
    cf_graph_builder.addStmt(4);
    cf_graph_builder.addStmt(5);
    cf_graph_builder.addDummyNode(4);
    cf_graph_builder.setLastVisitedStmt(4);
    cf_graph_builder.addStmt(6);
    cf_graph_builder.addDummyNode(4);
    cf_graph_builder.setMaxStmtNum(6);
    cf_graph_builder.setMinStmtNum(4);
    cf_graph_builder.setProcName("proc");
    CFG::CFGraph cf_graph2 = cf_graph_builder.build();
    {
        STMT_STMT_SET allPairs = cf_graph2.getPairwiseControlFlow(false);
        expected.insert(allPairs.begin(), allPairs.end());
        STMT_STMT_SET allPairsT = cf_graph2.getPairwiseControlFlow(true);
        expectedT.insert(allPairsT.begin(), allPairsT.end());
    }
    test.emplace_back(std::move(cf_graph2));
    CFG::CFGManager manager;
    manager.setGraphs(std::move(test));

    SECTION("4.1 transitive and non-transitive getValidNextPairs") {
        requireEqualRef(expected, manager.getValidNextPairs(false));
        requireEqualRef(expectedT, manager.getValidNextPairs(true));
    }

    SECTION("4.2 predecessors") {
        STMT_SET expectedPredecessors = {1, 2, 3, 4};
        requireEqualRef(expectedPredecessors, manager.getValidPredecessors());
    }

    SECTION("4.2 successors") {
        STMT_SET expectedSuccessors = {1, 2, 3, 5, 6};
        requireEqualRef(expectedSuccessors, manager.getValidSuccessors());
    }
}
