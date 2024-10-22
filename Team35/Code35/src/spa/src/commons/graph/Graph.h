#pragma once

#include <algorithm>
#include <cstdint>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "commons/types.h"

using std::unordered_set;

typedef uint32_t Index;
typedef unordered_set<Index> IndexSet;
typedef std::queue<Index> IndexQueue;

/**
 * A directed unweighted graph with nodes of type T. The nodes are unique by value. It is possible that one node is
 * connected to itself.
 */
template<typename T>
class Graph {
 public:
    Graph() : nodes(), node_to_index_(), outgoing_adj_list_(), incoming_adj_list_() {}

    virtual ~Graph() = default;

    Graph(Graph &&other) noexcept = default;

    Graph &operator=(Graph &&other) noexcept = default;

    Graph(const Graph &other) = default;

    Graph &operator=(const Graph &other) = default;

    bool operator==(const Graph &other) const {
        return nodes == other.nodes && node_to_index_ == other.node_to_index_ &&
               outgoing_adj_list_ == other.outgoing_adj_list_ && incoming_adj_list_ == other.incoming_adj_list_;
    }

    bool operator!=(const Graph &other) const {
        return !(*this == other);
    }

    void addEdge(const T &from, const T &to) {
        Index from_index = addNodeHelper(from);
        Index to_index = addNodeHelper(to);
        outgoing_adj_list_[from_index].insert(to_index);
        incoming_adj_list_[to_index].insert(from_index);
    }

    void addNode(const T &node) {
        addNodeHelper(node);
    }

    bool hasNode(const T &node) const {
        return node_to_index_.find(node) != node_to_index_.end();
    }

    bool isNeighbor(const T &node1, const T &node2) const {
        if (!hasNode(node1) || !hasNode(node2)) {
            return false;
        }
        Index node_index1 = getNodeIndex(node1);
        Index node_index2 = getNodeIndex(node2);
        return isNeighbor(node_index1, node_index2);
    }

    bool isCyclic() const {
        std::vector<bool> visited(nodes.size(), false);
        std::vector<bool> recursion_stack(nodes.size(), false);
        for (Index i = 0; i < nodes.size(); ++i) {
            if (isCyclicHelper(i, visited, recursion_stack)) {
                return true;
            }
        }
        return false;
    }

    uint32_t getNoOfNodes() const {
        return nodes.size();
    }

    uint32_t getNoOfOutgoingEdges(const T &node) const {
        if (!hasNode(node)) {
            return 0;
        }
        Index node_index = getNodeIndex(node);
        return outgoing_adj_list_.at(node_index).size();
    }

    uint32_t getNoOfIncomingEdges(const T &node) const {
        if (!hasNode(node)) {
            return 0;
        }
        Index node_index = getNodeIndex(node);
        return incoming_adj_list_.at(node_index).size();
    }

    unordered_set<T> getPredecessors(const T &node, UsageType usage) const {
        if (!this->hasNode(node)) {
            return {};
        }

        Index node_index = this->getNodeIndex(node);

        unordered_set<T> predecessors;

        if (usage == UsageType::Direct) {
            for (Index predecessor_index : this->getIncomingNodes(node_index)) {
                predecessors.insert(this->getNode(predecessor_index));
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
                predecessors.insert(this->getNode(predecessor_index));
            }
        }
        return predecessors;
    }

    unordered_set<T> getSuccessors(const T &node, UsageType usageType) const {
        if (!this->hasNode(node)) {
            return {};
        }

        Index node_index = this->getNodeIndex(node);

        unordered_set<T> successors;

        if (usageType == UsageType::Direct) {
            for (Index successor_index : this->getIncomingNodes(node_index)) {
                successors.insert(this->getNode(successor_index));
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
                successors.insert(this->getNode(successor_index));
            }
        }
        return successors;
    }

    bool isReachable(const T &node1, const T &node2, UsageType usageType) const {
        if (!this->hasNode(node1) || !this->hasNode(node2)) {
            return false;
        }
        Index node_index1 = this->getNodeIndex(node1);
        Index node_index2 = this->getNodeIndex(node2);

        if (usageType == UsageType::Direct) {
            return isNeighbor(node_index1, node_index2);
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

    virtual void reset() {
        nodes.clear();
        node_to_index_.clear();
        outgoing_adj_list_.clear();
        incoming_adj_list_.clear();
    }

 protected:
    bool isIndexValid(Index index) const {
        return index < nodes.size();
    }

    Index getNodeIndex(const T &node) const {
        return node_to_index_.at(node);
    }

    const T &getNode(Index index) const {
        return nodes.at(index);
    }

    bool isNeighbor(Index index1, Index index2) const {
        return outgoing_adj_list_.at(index1).find(index2) != outgoing_adj_list_.at(index1).end();
    }

    const IndexSet &getOutgoingNodes(Index index) const {
        return outgoing_adj_list_.at(index);
    }

    const IndexSet &getIncomingNodes(Index index) const {
        return incoming_adj_list_.at(index);
    }

    const IndexSet &getOutgoingNodes(const T &node) const {
        return outgoing_adj_list_.at(getNodeIndex(node));
    }

    const IndexSet &getIncomingNodes(const T &node) const {
        return incoming_adj_list_.at(getNodeIndex(node));
    }

 private:
    std::vector<T> nodes;
    std::unordered_map<T, Index> node_to_index_;
    std::vector<IndexSet> outgoing_adj_list_;
    std::vector<IndexSet> incoming_adj_list_;

    Index addNodeHelper(const T &node) {
        if (node_to_index_.find(node) != node_to_index_.end()) {
            return node_to_index_[node];
        }
        Index index = nodes.size();
        nodes.push_back(node);
        node_to_index_[node] = index;
        outgoing_adj_list_.emplace_back();
        incoming_adj_list_.emplace_back();
        return index;
    }

    bool isCyclicHelper(Index node, std::vector<bool> &visited, std::vector<bool> &recursion_stack) const {
        if (!visited[node]) {
            visited[node] = true;
            recursion_stack[node] = true;
            for (Index neighbour : getOutgoingNodes(node)) {
                if (!visited[neighbour] && isCyclicHelper(neighbour, visited, recursion_stack)) {
                    return true;
                } else if (recursion_stack[neighbour]) {
                    return true;
                }
            }
        }
        recursion_stack[node] = false;
        return false;
    }
};
