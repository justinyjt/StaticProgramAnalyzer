#include "CFGraphBuilder.h"

#include <utility>

namespace CFG {
CFGraphBuilder::CFGraphBuilder()
    : cf_graph_(),
      last_visited_node_data_(CFGraph::start_node_data),
      proc_name_(),
      min_stmt_num_(std::nullopt),
      max_stmt_num_(std::nullopt) {}

/**
 * Builds the control flow graph. This put the end node as the last node in the graph, if there is a last visited node.
 * @return The control flow graph.
 */
CFGraph CFGraphBuilder::build() {
    this->addNode(CFGraph::end_node_data);
    CFGraph cf_graph(this->cf_graph_, min_stmt_num_.value(), max_stmt_num_.value(), std::move(proc_name_));
    return std::move(cf_graph);
}

/**
 * Resets the control flow graph builder.
 */
void CFGraphBuilder::reset() {
    this->cf_graph_ = CFGraph();
    this->last_visited_node_data_ = CFGraph::start_node_data;
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
 * stmt_num is already added to the graph and assumes that there is a last visited node. Adds a dummy node to the
 * control flow graph after the loop.
 * @param stmt_num Statement number of the node to loop back to.
 */
void CFGraphBuilder::addLoop(STMT_NUM stmt_num) {
    CFGraphNodeData node_data = makeNodeData(stmt_num);
    this->addNode(node_data);
    this->addDummyNode(stmt_num);
}

/**
 * Sets the last visited node to the node with the given statement number. This assumes that stmt_num is already added
 * to the graph.
 * @param stmt_num Statement number of the node to set as the last visited node.
 */
void CFGraphBuilder::setLastVisitedStmt(STMT_NUM stmt_num) {
    CFGraphNodeData node_data = makeNodeData(stmt_num);
    this->setLastVisitedNode(node_data);
}

/**
 * Adds a dummy node to the control flow graph. Moves the last visited node to the new dummy node.
 * @param stmt_num Statement number of the dummy node.
 */
void CFGraphBuilder::addDummyNode(STMT_NUM stmt_num) {
    CFGraphNodeData dummy_node_data = makeDummyNodeData(stmt_num);
    this->addNode(dummy_node_data);
}

/**
 * Adds a node to the control flow graph. Moves the last visited node to the new node.
 * @param node_data node data of the node to add.
 */
void CFGraphBuilder::addNode(const CFGraphNodeData &node_data) {
    this->cf_graph_.addEdge(this->last_visited_node_data_, node_data);
    setLastVisitedNode(node_data);
}

void CFGraphBuilder::setLastVisitedNode(const CFGraphNodeData &node_data) {
    this->last_visited_node_data_ = node_data;
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

bool CFGraphBuilder::operator==(const CFGraphBuilder &builder) const {
    return this->cf_graph_ == builder.cf_graph_ &&
        this->last_visited_node_data_ == builder.last_visited_node_data_ &&
        this->proc_name_ == builder.proc_name_ &&
        this->min_stmt_num_ == builder.min_stmt_num_ &&
        this->max_stmt_num_ == builder.max_stmt_num_;
}

bool CFGraphBuilder::operator!=(const CFGraphBuilder &builder) const {
    return !(*this == builder);
}
}  // namespace CFG
