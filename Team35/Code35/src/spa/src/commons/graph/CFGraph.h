#pragma once

#include "CFGraphNodeData.h"
#include "Graph.h"
#include "commons/types.h"

namespace CFG {
class CFGraph : public Graph<CFGraphNodeData> {
 public:
    static CFGraphNodeData end_node_data;
    CFGraph();
    CFGraph(const CFGraph &graph, STMT_NUM min_stmt_num, STMT_NUM max_stmt_num, ENT_NAME proc_name);
    STMT_SET getPredecessors(STMT_NUM stmt_num, bool isTransitive) const;
    STMT_SET getSuccessors(STMT_NUM stmt_num, bool isTransitive) const;
    STMT_STMT_SET getPairwiseCFG(bool isTransitive) const;
    bool isReachable(STMT_NUM stmt1, STMT_NUM stmt2) const;
    STMT_NUM getMaxStmtNum() const;
    STMT_NUM getMinStmtNum() const;
    ENT_NAME getProcName() const;

 private:
    STMT_NUM max_stmt_num_;
    STMT_NUM min_stmt_num_;
    ENT_NAME proc_name_;
};
}  // namespace CFG
