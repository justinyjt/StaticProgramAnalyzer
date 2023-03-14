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
    /**
     * 1,
     * 2,
     * 3,
     * 4,
     * 5,
     * 6,
     * 7,
     * 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
     */
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
    cf_graph_builder.addStmt(7);
    cf_graph_builder.addStmt(8);
    cf_graph_builder.addDummyNode(7);
    cf_graph_builder.setLastVisitedStmt(7);
    cf_graph_builder.addStmt(9);
    cf_graph_builder.addDummyNode(7);
    cf_graph_builder.addStmt(10);
    cf_graph_builder.addStmt(11);
    cf_graph_builder.addDummyNode(10);
    cf_graph_builder.setLastVisitedStmt(10);
    cf_graph_builder.addStmt(12);
    cf_graph_builder.addDummyNode(10);
    cf_graph_builder.addStmt(13);
    cf_graph_builder.addStmt(14);
    cf_graph_builder.addDummyNode(13);
    cf_graph_builder.setLastVisitedStmt(13);
    cf_graph_builder.addStmt(15);
    cf_graph_builder.addDummyNode(13);
    cf_graph_builder.addStmt(16);
    cf_graph_builder.addStmt(17);
    cf_graph_builder.addDummyNode(16);
    cf_graph_builder.setLastVisitedStmt(16);
    cf_graph_builder.addStmt(17);
    cf_graph_builder.addDummyNode(16);

    for (int i = 1; i < 20; ++i) {
        cf_graph_builder.addStmt(i);
        if (i % 2 == 0) {
            cf_graph_builder.addDummyNode(i - 1);
            cf_graph_builder.setLastVisitedStmt(i - 1);

        }
    }
    cf_graph_builder.setMaxStmtNum(19);
    cf_graph_builder.setMinStmtNum(1);
    cf_graph_builder.setProcName("proc");
    CFG::CFGraph cf_graph = cf_graph_builder.build();
    std::vector<CFG::CFGraph> test;
    test.emplace_back(std::move(cf_graph));
    manager.setGraphs(std::move(test));
    SECTION("Test is next exist") {
        requireTrue(manager.isNextExists());
    }

    SECTION("Test is next exist by key") {
        requireTrue(manager.isNextExistAfterStmtNum(2, true));
        for (int i = 1; i < 19; ++i) {
            requireTrue(manager.isNextExistAfterStmtNum(i, true));
            requireTrue(manager.isNextExistAfterStmtNum(i, false));
            STMT_SET expected;
            if (i % 2 == 0) {
                expected.emplace(i - 1);

            } else {
                expected.emplace(i + 1);
                expected.emplace(i + 2);
            }
            requireEqual(expected, manager.getConnectedStmts(i, true, false));
        }
        requireFalse(manager.isNextExistAfterStmtNum(20, true));
        requireFalse(manager.isNextExistAfterStmtNum(20, false));
    }

//    SECTION("Test is next exist by value") {
//        for (int i = 2; i < 20; ++i) {
//            requireTrue(manager.isNextExistByVal(i, true));
//            requireTrue(manager.isNextExistByVal(i, false));
//        }
//        requireFalse(manager.isNextExistByVal(1, true));
//        requireFalse(manager.isNextExistByVal(1, false));
//
//    }
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
