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
    CFGraphNodeData node_data = makeNodeData(stmt_num);
    if (!this->hasNode(node_data)) {
        return {};
    }

    STMT_SET successors;
    return STMT_SET();
}

STMT_STMT_SET CFGraph::getPairwiseCFG(bool isTransitive) const {
    return STMT_STMT_SET();
}

bool CFGraph::isReachable(STMT_NUM stmt1, STMT_NUM stmt2) const {
    bool stmt_num_out_of_range =
        (stmt1 < min_stmt_num_ || stmt1 > max_stmt_num_ || stmt2 < min_stmt_num_ || stmt2 > max_stmt_num_);
    if (stmt_num_out_of_range) {
        return false;
    }
    CFGraphNodeData node_data1 = makeNodeData(stmt1);
    CFGraphNodeData node_data2 = makeNodeData(stmt2);
    if (!this->hasNode(node_data1) || !this->hasNode(node_data2)) {
        return false;
    }
    Index node_index1 = this->getNodeIndex(node_data1);
    Index node_index2 = this->getNodeIndex(node_data2);

    IndexQueue frontier;
    IndexSet visited;
    frontier.push(node_index1);

    while (!frontier.empty()) {
        Index node_index = frontier.front();
        frontier.pop();
        visited.insert(node_index);
        for (Index successor_index : this->getOutgoingNodes(node_index)) {
            if (successor_index == node_index2) {
                return true;
            }
            if (visited.find(successor_index) == visited.end()) {
                frontier.push(successor_index);
            }
        }
    }

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
