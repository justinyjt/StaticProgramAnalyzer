#include "CFGraph.h"

#include <utility>

#include "CFGraphNodeData.h"

namespace CFG {
CFGraphNodeData CFGraph::start_node_data = makeDummyNodeData(0);
CFGraphNodeData CFGraph::end_node_data = makeDummyNodeData(-1);

CFGraph::CFGraph() : Graph<CFGraphNodeData>(),
                     min_stmt_num_(0),
                     max_stmt_num_(0),
                     proc_name_(),
                     pairwise_control_flow_transitive_(std::nullopt),
                     pairwise_control_flow_non_transitive_(std::nullopt) {
    this->addNode(CFGraph::start_node_data);
    this->addNode(CFGraph::end_node_data);
}

CFGraph::CFGraph(const CFGraph &graph, STMT_NUM min_stmt_num, STMT_NUM max_stmt_num, ENT_NAME proc_name) :
        Graph<CFGraphNodeData>(graph),
        max_stmt_num_(max_stmt_num),
        min_stmt_num_(min_stmt_num),
        proc_name_(std::move(proc_name)),
        pairwise_control_flow_transitive_(graph.pairwise_control_flow_transitive_),
        pairwise_control_flow_non_transitive_(graph.pairwise_control_flow_non_transitive_) {
    this->addNode(CFGraph::start_node_data);
    this->addNode(CFGraph::end_node_data);
}

STMT_SET CFGraph::getPredecessors(STMT_NUM stmt_num, bool isTransitive) const {
    CFGraphNodeData node_data = makeNodeData(stmt_num);
    if (!this->hasNode(node_data)) {
        return {};
    }

    Index node_index = this->getNodeIndex(node_data);

    return this->getPredecessorsByIndex(node_index, isTransitive);
}

STMT_SET CFGraph::getSuccessors(STMT_NUM stmt_num, bool isTransitive) const {
    CFGraphNodeData node_data = makeNodeData(stmt_num);
    if (!this->hasNode(node_data)) {
        return {};
    }

    Index node_index = this->getNodeIndex(node_data);

    return this->getSuccessorsByIndex(node_index, isTransitive);
}

STMT_SET CFGraph::getAllSuccessors() const {
    assert(this->hasNode(CFGraph::start_node_data));
    assert(this->hasNode(CFGraph::end_node_data));
    STMT_SET result;
    Index node_index = this->getNodeIndex(CFGraph::start_node_data);
    IndexSet successors = getDummyNodeSuccessors(node_index);
    for (const auto &index : successors) {
        STMT_SET current = this->getSuccessorsByIndex(index, true);
        result.insert(current.begin(), current.end());
    }
    return result;
}

STMT_SET CFGraph::getAllPredecessors() const {
    assert(this->hasNode(CFGraph::start_node_data));
    assert(this->hasNode(CFGraph::end_node_data));
    STMT_SET result;
    Index node_index = this->getNodeIndex(CFGraph::end_node_data);
    IndexSet predecessors = getDummyNodePredecessors(node_index);
    for (const auto &index : predecessors) {
        STMT_SET current = this->getPredecessorsByIndex(index, true);
        result.insert(current.begin(), current.end());
    }
    return result;
}

STMT_SET CFGraph::getSuccessorsByIndex(Index node_index, bool isTransitive) const {
    assert(this->isIndexValid(node_index));
    STMT_SET successors;

    if (!isTransitive) {
        for (Index successor_index : this->getOutgoingNodes(node_index)) {
            if (!isIndexDummyNode(successor_index)) {
                successors.insert(this->getStmtNumFromIndex(successor_index));
            } else {
                for (Index successor_of_dummy_node_index : this->getDummyNodeSuccessors(successor_index)) {
                    successors.insert(this->getStmtNumFromIndex(successor_of_dummy_node_index));
                }
            }
        }
        return successors;
    }
    IndexQueue frontier;
    IndexSet visited;
    frontier.push(node_index);

    while (!frontier.empty()) {
        Index index = frontier.front();
        frontier.pop();
        visited.insert(index);
        for (Index successor_index : this->getOutgoingNodes(index)) {
            if (visited.find(successor_index) == visited.end()) {
                frontier.push(successor_index);
            }
            if (!isIndexDummyNode(successor_index)) {
                successors.insert(this->getStmtNumFromIndex(successor_index));
            }
        }
    }
    return successors;
}

STMT_SET CFGraph::getPredecessorsByIndex(Index node_index, bool isTransitive) const {
    assert(this->isIndexValid(node_index));
    STMT_SET predecessors;

    if (!isTransitive) {
        for (Index predecessor_index : this->getIncomingNodes(node_index)) {
            if (!isIndexDummyNode(predecessor_index)) {
                predecessors.insert(this->getStmtNumFromIndex(predecessor_index));
            } else {
                for (Index predecessor_of_dummy_index : this->getDummyNodePredecessors(predecessor_index)) {
                    predecessors.insert(this->getStmtNumFromIndex(predecessor_of_dummy_index));
                }
            }
        }
        return predecessors;
    }
    IndexQueue frontier;
    IndexSet visited;
    frontier.push(node_index);

    while (!frontier.empty()) {
        Index index = frontier.front();
        frontier.pop();
        visited.insert(index);
        for (Index predecessor_index : this->getIncomingNodes(index)) {
            if (visited.find(predecessor_index) == visited.end()) {
                frontier.push(predecessor_index);
            }
            if (!isIndexDummyNode(predecessor_index)) {
                predecessors.insert(this->getStmtNumFromIndex(predecessor_index));
            }
        }
    }
    return predecessors;
}

const STMT_STMT_SET &CFGraph::getPairwiseControlFlow(bool isTransitive) {
    // memoization
    if (isTransitive && this->pairwise_control_flow_transitive_.has_value()) {
        return this->pairwise_control_flow_transitive_.value();
    }

    if (!isTransitive && this->pairwise_control_flow_non_transitive_.has_value()) {
        return this->pairwise_control_flow_non_transitive_.value();
    }

    std::optional<STMT_STMT_SET> *pairwise_control_flow =
            isTransitive ? &(this->pairwise_control_flow_transitive_) : &(this->pairwise_control_flow_non_transitive_);

    *pairwise_control_flow = STMT_STMT_SET();
    for (Index node_index = 0; node_index < this->getNoOfNodes(); ++node_index) {
        if (isIndexDummyNode(node_index)) {
            continue;
        }
        STMT_NUM stmt_num = this->getStmtNumFromIndex(node_index);
        for (STMT_NUM successor : this->getSuccessorsByIndex(node_index, isTransitive)) {
            (*pairwise_control_flow)->emplace(stmt_num, successor);
        }
    }

    return pairwise_control_flow->value();
}

bool CFGraph::isReachable(STMT_NUM stmt1, STMT_NUM stmt2, bool check_neighbor_only) const {
    CFGraphNodeData node_data1 = makeNodeData(stmt1);
    CFGraphNodeData node_data2 = makeNodeData(stmt2);
    if (!this->hasNode(node_data1) || !this->hasNode(node_data2)) {
        return false;
    }
    Index node_index1 = this->getNodeIndex(node_data1);
    Index node_index2 = this->getNodeIndex(node_data2);

    if (check_neighbor_only) {
        for (Index successor_index : this->getOutgoingNodes(node_index1)) {
            if (successor_index == node_index2) {
                return true;
            }
            if (isIndexDummyNode(successor_index)) {
                IndexSet successors_of_dummy_node = this->getDummyNodeSuccessors(successor_index);
                if (std::find(successors_of_dummy_node.begin(), successors_of_dummy_node.end(), node_index2) !=
                    successors_of_dummy_node.end()) {
                    return true;
                }
            }
        }
        return false;
    }

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

bool CFGraph::isIndexDummyNode(Index index) const {
    assert(this->isIndexValid(index));
    return this->getNode(index).isDummy();
}

STMT_NUM CFGraph::getStmtNumFromIndex(Index index) const {
    assert(this->isIndexValid(index));
    return this->getNode(index).getStmtNum();
}

IndexSet CFGraph::getDummyNodePredecessors(Index index) const {
    assert(this->isIndexValid(index));
    assert(this->isIndexDummyNode(index));
    IndexSet predecessors;
    for (Index predecessor_index : this->getIncomingNodes(index)) {
        if (!this->isIndexDummyNode(predecessor_index)) {
            predecessors.insert(predecessor_index);
        } else {
            IndexSet dummy_predecessors = this->getDummyNodePredecessors(predecessor_index);
            predecessors.insert(dummy_predecessors.begin(), dummy_predecessors.end());
        }
    }
    return predecessors;
}

IndexSet CFGraph::getDummyNodeSuccessors(Index index) const {
    assert(this->isIndexValid(index));
    assert(this->isIndexDummyNode(index));
    IndexSet successors;
    for (Index successor_index : this->getOutgoingNodes(index)) {
        if (!this->isIndexDummyNode(successor_index)) {
            successors.insert(successor_index);
        } else {
            IndexSet dummy_successors = this->getDummyNodeSuccessors(successor_index);
            successors.insert(dummy_successors.begin(), dummy_successors.end());
        }
    }
    return successors;
}

bool CFGraph::operator==(const CFGraph &graph) const {
    return Graph<CFGraphNodeData>::operator==(graph) && this->proc_name_ == graph.proc_name_ &&
           this->min_stmt_num_ == graph.min_stmt_num_ && this->max_stmt_num_ == graph.max_stmt_num_;
}

bool CFGraph::operator!=(const CFGraph &graph) const {
    return !(*this == graph);
}
}  // namespace CFG
