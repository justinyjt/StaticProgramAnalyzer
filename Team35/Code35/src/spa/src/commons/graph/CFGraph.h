#pragma once

#include <optional>

#include "CFGraphNodeData.h"
#include "Graph.h"
#include "commons/types.h"

namespace CFG {
class CFGraph : public Graph<CFGraphNodeData> {
 public:
    // implement move semantics
    CFGraph(const CFGraph &graph) = delete;

    CFGraph &operator=(const CFGraph &graph) = delete;

    CFGraph(CFGraph &&graph) = default;

    CFGraph &operator=(CFGraph &&graph) = default;

    bool operator==(const CFGraph &graph) const;

    bool operator!=(const CFGraph &graph) const;

    static CFGraphNodeData end_node_data;
    static CFGraphNodeData start_node_data;

    CFGraph();

    ~CFGraph() override = default;

    CFGraph(const CFGraph &graph, STMT_NUM min_stmt_num, STMT_NUM max_stmt_num, ENT_NAME proc_name);

    STMT_SET getPredecessors(STMT_NUM stmt_num, bool isTransitive) const;

    STMT_SET getSuccessors(STMT_NUM stmt_num, bool isTransitive) const;

    const STMT_STMT_SET &getPairwiseControlFlow(bool isTransitive);

    bool isReachable(STMT_NUM stmt1, STMT_NUM stmt2) const;

    STMT_NUM getMaxStmtNum() const;

    STMT_NUM getMinStmtNum() const;

    ENT_NAME getProcName() const;

 private:
    STMT_NUM max_stmt_num_;
    STMT_NUM min_stmt_num_;
    ENT_NAME proc_name_;
    std::optional<STMT_STMT_SET> pairwise_control_flow_transitive_;
    std::optional<STMT_STMT_SET> pairwise_control_flow_non_transitive_;

    bool isIndexDummyNode(Index index) const;

    STMT_NUM getStmtNumFromIndex(Index index) const;

    IndexList getDummyNodePredecessors(Index index) const;

    IndexList getDummyNodeSuccessors(Index index) const;

    STMT_SET getSuccessorsByIndex(Index index, bool isTransitive) const;
};
}  // namespace CFG
