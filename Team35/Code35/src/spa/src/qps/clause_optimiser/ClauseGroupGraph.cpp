#include "ClauseGroupGraph.h"

#include <utility>

ClauseGroupGraphNode::ClauseGroupGraphNode(const ClauseIndex &clause_index)
        : type(Type::CLAUSE), clause_index(clause_index) {}

ClauseGroupGraphNode::ClauseGroupGraphNode(HEADER header)
        : type(Type::HEADER), header(std::move(header)), clause_index(0) {}

bool ClauseGroupGraphNode::operator==(const ClauseGroupGraphNode &rhs) const {
    if (type == Type::CLAUSE) {
        return type == rhs.type && clause_index == rhs.clause_index;
    } else {
        return type == rhs.type && header == rhs.header;
    }
}

bool ClauseGroupGraphNode::operator!=(const ClauseGroupGraphNode &rhs) const {
    return !(rhs == *this);
}

void ClauseGroupGraph::addClauseHeaderRelations(const ClauseIndex &clause_index, const HEADER_SET &headers) {
    if (headers.empty()) {
        return;
    }
    this->clause_indices_.insert(clause_index);
    for (const auto &header : headers) {
        this->addEdge(ClauseGroupGraphNode(clause_index), ClauseGroupGraphNode(header));
    }
}

std::vector<ClauseIndexList> ClauseGroupGraph::getClauseGroupsIndices() const {
    std::vector<ClauseIndexList> clause_groups_indices;
    ClauseIndexSet visited_clause_indices;
    for (const auto &clause_index : this->clause_indices_) {
        if (visited_clause_indices.find(clause_index) != visited_clause_indices.end()) {
            continue;
        }
        clause_groups_indices.emplace_back(getSingleClauseGroupIndices(clause_index, visited_clause_indices));
    }
    return clause_groups_indices;
}

ClauseIndexList ClauseGroupGraph::getSingleClauseGroupIndices(const ClauseIndex &clause_index,
                                                              ClauseIndexSet &visited_clause_indices) const {
    ClauseIndexList clause_group_indices;
    ClauseIndexQueue frontier;
    frontier.push(clause_index);
    while (!frontier.empty()) {
        auto current_clause_index = frontier.front();
        frontier.pop();
        if (visited_clause_indices.find(current_clause_index) != visited_clause_indices.end()) {
            continue;
        }
        clause_group_indices.push_back(current_clause_index);
        visited_clause_indices.insert(current_clause_index);
        for (const auto &neighboring_clause_index : this->getNeighboringClauseIndices(current_clause_index)) {
            frontier.push(neighboring_clause_index);
        }
    }
    return std::move(clause_group_indices);
}

ClauseIndexSet ClauseGroupGraph::getNeighboringClauseIndices(const ClauseIndex &clause_index) const {
    ClauseIndexSet neighboring_clause_indices;
    for (const auto &header_node_index : this->getOutgoingNodes(ClauseGroupGraphNode(clause_index))) {
        for (const auto &clause_node_index : this->getIncomingNodes(header_node_index)) {
            auto &neighboring_clause_index = this->getClauseIndex(clause_node_index);
            if (neighboring_clause_index != clause_index) {
                neighboring_clause_indices.insert(neighboring_clause_index);
            }
        }
    }
    return neighboring_clause_indices;
}

const ClauseIndex &ClauseGroupGraph::getClauseIndex(const Index &clause_node_index) const {
    return this->getNode(clause_node_index).clause_index;
}
