#include "CFGraph.h"

#include <utility>

#include "CFGraphNodeData.h"

namespace CFG {
CFGraphNodeData CFGraph::end_node_data = makeDummyNodeData(0);

CFGraph::CFGraph() : Graph<CFGraphNodeData>(), min_stmt_num_(), max_stmt_num_(), proc_name_() {}

CFGraph::CFGraph(const CFGraph &graph, STMT_NUM min_stmt_num, STMT_NUM max_stmt_num, ENT_NAME proc_name) :
    Graph<CFGraphNodeData>(graph),
    max_stmt_num_(max_stmt_num),
    min_stmt_num_(min_stmt_num),
    proc_name_(std::move(proc_name)) {}

STMT_SET CFGraph::getPredecessors(STMT_NUM stmt_num, bool isTransitive) const {
    return STMT_SET();
}

STMT_SET CFGraph::getSuccessors(STMT_NUM stmt_num, bool isTransitive) const {
    return STMT_SET();
}

STMT_STMT_SET CFGraph::getPairwiseCFG(bool isTransitive) const {
    return STMT_STMT_SET();
}

bool CFGraph::isReachable(STMT_NUM stmt1, STMT_NUM stmt2) const {
    return false;
}

STMT_NUM CFGraph::getMaxStmtNum() const {
    return max_stmt_num_;
}

STMT_NUM CFGraph::getMinStmtNum() const {
    return min_stmt_num_;
}

ENT_NAME CFGraph::getProcName() const {
    return proc_name_;
}
}  // namespace CFG
