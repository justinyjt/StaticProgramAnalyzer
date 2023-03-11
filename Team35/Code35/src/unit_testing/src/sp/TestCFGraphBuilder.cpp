#include "catch.hpp"
#include "../TestHelper.h"
#include "sp/CFGraphBuilder.h"
#include "commons/graph/CFGraph.h"

TEST_CASE("1. test reset") {
    CFG::CFGraphBuilder cf_graph_builder;
    cf_graph_builder.addStmt(1);
    cf_graph_builder.addStmt(2);
    cf_graph_builder.setMaxStmtNum(2);
    cf_graph_builder.setMinStmtNum(1);
    cf_graph_builder.setProcName("proc");
    cf_graph_builder.reset();
    CFG::CFGraphBuilder cf_graph_builder2;
    requireTrue(cf_graph_builder2 == cf_graph_builder);
}

TEST_CASE("2. test addStmt") {
    SECTION("2.1. multiple stmt") {
        CFG::CFGraphBuilder cf_graph_builder;
        cf_graph_builder.addStmt(1);
        cf_graph_builder.addStmt(2);
        cf_graph_builder.setMaxStmtNum(2);
        cf_graph_builder.setMinStmtNum(1);
        cf_graph_builder.setProcName("proc");
        CFG::CFGraph cf_graph = cf_graph_builder.build();
        CFG::CFGraph cf_graph2(CFG::CFGraph(), 1, 2, "proc");
        cf_graph2.addEdge(CFG::CFGraph::start_node_data, CFG::makeNodeData(1));
        cf_graph2.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
        cf_graph2.addEdge(CFG::makeNodeData(2), CFG::CFGraph::end_node_data);
        requireEqualRef(cf_graph, cf_graph2);
    }

    SECTION("2.2. single stmt") {
        CFG::CFGraphBuilder cf_graph_builder;
        cf_graph_builder.addStmt(1);
        cf_graph_builder.setMaxStmtNum(2);
        cf_graph_builder.setMinStmtNum(1);
        cf_graph_builder.setProcName("proc");
        CFG::CFGraph cf_graph = cf_graph_builder.build();
        CFG::CFGraph cf_graph2(CFG::CFGraph(), 1, 2, "proc");
        cf_graph2.addEdge(CFG::CFGraph::start_node_data, CFG::makeNodeData(1));
        cf_graph2.addEdge(CFG::makeNodeData(1), CFG::CFGraph::end_node_data);
        requireEqualRef(cf_graph, cf_graph2);
    }
}

TEST_CASE("4. test addDummyNode, not the start node") {
    SECTION("4.1. not the start node") {
        CFG::CFGraphBuilder cf_graph_builder;
        cf_graph_builder.addStmt(1);
        cf_graph_builder.addDummyNode(2);
        cf_graph_builder.addStmt(3);
        cf_graph_builder.setMaxStmtNum(3);
        cf_graph_builder.setMinStmtNum(1);
        cf_graph_builder.setProcName("proc");
        CFG::CFGraph cf_graph = cf_graph_builder.build();
        CFG::CFGraph cf_graph2(CFG::CFGraph(), 1, 3, "proc");
        cf_graph2.addEdge(CFG::CFGraph::start_node_data, CFG::makeNodeData(1));
        cf_graph2.addEdge(CFG::makeNodeData(1), CFG::makeDummyNodeData(2));
        cf_graph2.addEdge(CFG::makeDummyNodeData(2), CFG::makeNodeData(3));
        cf_graph2.addEdge(CFG::makeNodeData(3), CFG::CFGraph::end_node_data);
        requireEqualRef(cf_graph, cf_graph2);
    }

    SECTION("4.2. start node") {
        CFG::CFGraphBuilder cf_graph_builder;
        cf_graph_builder.addDummyNode(1);
        cf_graph_builder.addStmt(2);
        cf_graph_builder.setMaxStmtNum(2);
        cf_graph_builder.setMinStmtNum(1);
        cf_graph_builder.setProcName("proc");
        CFG::CFGraph cf_graph = cf_graph_builder.build();
        CFG::CFGraph cf_graph2(CFG::CFGraph(), 1, 2, "proc");
        cf_graph2.addEdge(CFG::CFGraph::start_node_data, CFG::makeDummyNodeData(1));
        cf_graph2.addEdge(CFG::makeDummyNodeData(1), CFG::makeNodeData(2));
        cf_graph2.addEdge(CFG::makeNodeData(2), CFG::CFGraph::end_node_data);
        requireEqualRef(cf_graph, cf_graph2);
    }
}

TEST_CASE("5. test addLoop() method") {
    CFG::CFGraphBuilder cf_graph_builder;
    cf_graph_builder.addStmt(1);
    cf_graph_builder.addStmt(2);
    cf_graph_builder.addLoop(1);
    cf_graph_builder.setMaxStmtNum(2);
    cf_graph_builder.setMinStmtNum(1);
    cf_graph_builder.setProcName("proc");
    CFG::CFGraph cf_graph = cf_graph_builder.build();
    CFG::CFGraph cf_graph2(CFG::CFGraph(), 1, 2, "proc");
    cf_graph2.addEdge(CFG::CFGraph::start_node_data, CFG::makeNodeData(1));
    cf_graph2.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
    cf_graph2.addEdge(CFG::makeNodeData(2), CFG::makeNodeData(1));
    cf_graph2.addEdge(CFG::makeNodeData(1), CFG::makeDummyNodeData(1));
    cf_graph2.addEdge(CFG::makeDummyNodeData(1), CFG::CFGraph::end_node_data);
    requireEqualRef(cf_graph, cf_graph2);
}

TEST_CASE("6. test setLastVisitedStmt method") {
    CFG::CFGraphBuilder cf_graph_builder;
    cf_graph_builder.addStmt(1);
    cf_graph_builder.addStmt(2);
    cf_graph_builder.setLastVisitedStmt(1);
    cf_graph_builder.addStmt(3);
    cf_graph_builder.setMaxStmtNum(3);
    cf_graph_builder.setMinStmtNum(1);
    cf_graph_builder.setProcName("proc");
    CFG::CFGraph cf_graph = cf_graph_builder.build();
    CFG::CFGraph cf_graph2(CFG::CFGraph(), 1, 3, "proc");
    cf_graph2.addEdge(CFG::CFGraph::start_node_data, CFG::makeNodeData(1));
    cf_graph2.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(2));
    cf_graph2.addEdge(CFG::makeNodeData(1), CFG::makeNodeData(3));
    cf_graph2.addEdge(CFG::makeNodeData(3), CFG::CFGraph::end_node_data);
    requireEqualRef(cf_graph, cf_graph2);
}

TEST_CASE("7. test == / != operator for CFGraphBuilder") {
    SECTION("7.1. equality") {
        CFG::CFGraphBuilder cf_graph_builder;
        cf_graph_builder.addStmt(1);
        cf_graph_builder.addStmt(2);
        cf_graph_builder.setMaxStmtNum(2);
        cf_graph_builder.setMinStmtNum(1);
        cf_graph_builder.setProcName("proc");
        CFG::CFGraphBuilder cf_graph_builder2;
        cf_graph_builder2.addStmt(1);
        cf_graph_builder2.addStmt(2);
        cf_graph_builder2.setMaxStmtNum(2);
        cf_graph_builder2.setMinStmtNum(1);
        cf_graph_builder2.setProcName("proc");
        requireTrue(cf_graph_builder == cf_graph_builder2);
        requireFalse(cf_graph_builder != cf_graph_builder2);
    }

    SECTION("7.2. inequality - different proc name") {
        CFG::CFGraphBuilder cf_graph_builder;
        cf_graph_builder.addStmt(1);
        cf_graph_builder.addStmt(2);
        cf_graph_builder.setMaxStmtNum(2);
        cf_graph_builder.setMinStmtNum(1);
        cf_graph_builder.setProcName("proc");
        CFG::CFGraphBuilder cf_graph_builder2;
        cf_graph_builder2.addStmt(1);
        cf_graph_builder2.addStmt(2);
        cf_graph_builder2.setMaxStmtNum(2);
        cf_graph_builder2.setMinStmtNum(1);
        cf_graph_builder2.setProcName("proc2");
        requireFalse(cf_graph_builder == cf_graph_builder2);
        requireTrue(cf_graph_builder != cf_graph_builder2);
    }

    SECTION("7.3. inequality - different max stmt num") {
        CFG::CFGraphBuilder cf_graph_builder;
        cf_graph_builder.addStmt(1);
        cf_graph_builder.addStmt(2);
        cf_graph_builder.setMaxStmtNum(2);
        cf_graph_builder.setMinStmtNum(1);
        cf_graph_builder.setProcName("proc");
        CFG::CFGraphBuilder cf_graph_builder2;
        cf_graph_builder2.addStmt(1);
        cf_graph_builder2.addStmt(2);
        cf_graph_builder2.setMaxStmtNum(3);
        cf_graph_builder2.setMinStmtNum(1);
        cf_graph_builder2.setProcName("proc");
        requireFalse(cf_graph_builder == cf_graph_builder2);
        requireTrue(cf_graph_builder != cf_graph_builder2);
    }

    SECTION("7.4. inequality - different min stmt num") {
        CFG::CFGraphBuilder cf_graph_builder;
        cf_graph_builder.addStmt(1);
        cf_graph_builder.addStmt(2);
        cf_graph_builder.setMaxStmtNum(2);
        cf_graph_builder.setMinStmtNum(1);
        cf_graph_builder.setProcName("proc");
        CFG::CFGraphBuilder cf_graph_builder2;
        cf_graph_builder2.addStmt(1);
        cf_graph_builder2.addStmt(2);
        cf_graph_builder2.setMaxStmtNum(2);
        cf_graph_builder2.setMinStmtNum(2);
        cf_graph_builder2.setProcName("proc");
        requireFalse(cf_graph_builder == cf_graph_builder2);
        requireTrue(cf_graph_builder != cf_graph_builder2);
    }

    SECTION("7.5. inequality - different stmts") {
        CFG::CFGraphBuilder cf_graph_builder;
        cf_graph_builder.addStmt(1);
        cf_graph_builder.addStmt(2);
        cf_graph_builder.setMaxStmtNum(2);
        cf_graph_builder.setMinStmtNum(1);
        cf_graph_builder.setProcName("proc");
        CFG::CFGraphBuilder cf_graph_builder2;
        cf_graph_builder2.addStmt(1);
        cf_graph_builder2.addStmt(3);
        cf_graph_builder2.setMaxStmtNum(2);
        cf_graph_builder2.setMinStmtNum(1);
        cf_graph_builder2.setProcName("proc");
        requireFalse(cf_graph_builder == cf_graph_builder2);
        requireTrue(cf_graph_builder != cf_graph_builder2);
    }
}
