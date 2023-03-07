#include "CFGraphBuilder.h"

#include <utility>

namespace CFG {
CFGraphBuilder::CFGraphBuilder()
    : cf_graph_(),
      last_visited_node_data_(std::nullopt),
      proc_name_(),
      min_stmt_num_(std::nullopt),
      max_stmt_num_(std::nullopt) {}

/**
 * Builds the control flow graph. This put the end node as the last node in the graph, if there is a last visited node.
 * @return The control flow graph.
 */
CFGraph CFGraphBuilder::build() {
    assert(min_stmt_num_.has_value());
    assert(max_stmt_num_.has_value());
    assert(!proc_name_.empty());
    this->addNode(CFGraph::end_node_data);
    CFGraph cf_graph(this->cf_graph_, min_stmt_num_.value(), max_stmt_num_.value(), std::move(proc_name_));
    return std::move(cf_graph);
}

/**
 * Resets the control flow graph builder.
 */
void CFGraphBuilder::reset() {
    this->cf_graph_ = CFGraph();
    this->last_visited_node_data_ = std::nullopt;
    this->proc_name_ = "";
    this->min_stmt_num_ = std::nullopt;
    this->max_stmt_num_ = std::nullopt;
}

/**
 * Adds a statement node to the control flow graph. Moves the last visited node to the new node.
 * @param stmt_num Statement number of the statement node.
 */
void CFGraphBuilder::addStmt(STMT_NUM stmt_num) {
    CFGraphNodeData node_data = makeNodeData(stmt_num);
    this->addNode(node_data);
}

/**
 * Adds a loop back to a node to the control flow graph. Moves the last visited node to the new node. This assumes that
 * stmt_num is already added to the graph and assumes that there is a last visited node.
 * @param stmt_num Statement number of the node to loop back to.
 */
void CFGraphBuilder::addLoop(STMT_NUM stmt_num) {
    assert(isLastVisitedNodeExist());
    CFGraphNodeData node_data = makeNodeData(stmt_num);
    this->addNode(node_data);
    this->addDummyNode(stmt_num);
}

/**
 * Adds a dummy node to the control flow graph. Does not move the last visited node to the new node. This assumes that
 * there is a last visited node.
 * @param stmt_num Statement number of the dummy node.
 */
void CFGraphBuilder::linkToDummyNode(STMT_NUM stmt_num) {
    assert(isLastVisitedNodeExist());
    CFGraphNodeData dummy_node_data = makeDummyNodeData(stmt_num);
    this->addNode(dummy_node_data);
}

/**
 * Adds a dummy node to the control flow graph. Moves the last visited node to the new dummy node.
 * @param stmt_num Statement number of the dummy node.
 */
void CFGraphBuilder::addDummyNode(STMT_NUM stmt_num) {
    CFGraphNodeData dummy_node_data = makeDummyNodeData(stmt_num);
    this->addNode(dummy_node_data);
}

void CFGraphBuilder::setLastVisitedNode(const CFGraphNodeData &node_data) {
    this->last_visited_node_data_ = node_data;
}

bool CFGraphBuilder::isLastVisitedNodeExist() const {
    return this->last_visited_node_data_.has_value();
}

void CFGraphBuilder::setProcName(ENT_NAME proc_name) {
    this->proc_name_ = std::move(proc_name);
}

void CFGraphBuilder::setMaxStmtNum(STMT_NUM max_stmt_num) {
    this->max_stmt_num_ = max_stmt_num;
}

void CFGraphBuilder::setMinStmtNum(STMT_NUM min_stmt_num) {
    this->min_stmt_num_ = min_stmt_num;
}
}  // namespace CFG
