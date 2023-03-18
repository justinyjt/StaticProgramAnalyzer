#pragma once

#include <cassert>
#include <cstdint>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef uint32_t Index;
typedef std::vector<Index> IndexList;
typedef std::unordered_set<Index> IndexSet;
typedef std::queue<Index> IndexQueue;

/**
 * A directed unweighted graph with nodes of type T. The nodes are unique by value. It is possible that one node is
 * connected to another node multiple times, or connected to itself.
 */
template<typename T>
class Graph {
 public:
    Graph() : nodes(), nodeToIndex(), outgoingAdjList(), incomingAdjList() {}

    virtual ~Graph() = default;

    Graph(Graph &&other) noexcept = default;

    Graph &operator=(Graph &&other) noexcept = default;

    Graph(const Graph &other) = default;

    Graph &operator=(const Graph &other) = default;

    bool operator==(const Graph &other) const {
        return nodes == other.nodes && nodeToIndex == other.nodeToIndex &&
            outgoingAdjList == other.outgoingAdjList && incomingAdjList == other.incomingAdjList;
    }

    bool operator!=(const Graph &other) const {
        return !(*this == other);
    }

    void addEdge(const T &from, const T &to) {
        Index fromIndex = addNodeHelper(from);
        Index toIndex = addNodeHelper(to);
        outgoingAdjList[fromIndex].push_back(toIndex);
        incomingAdjList[toIndex].push_back(fromIndex);
    }

    void addNode(const T &node) {
        addNodeHelper(node);
    }

    bool hasNode(const T &node) const {
        return nodeToIndex.find(node) != nodeToIndex.end();
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

    int getNoOfNodes() const {
        return nodes.size();
    }

 protected:
    bool isIndexValid(Index index) const {
        return index < nodes.size();
    }

    Index getNodeIndex(const T &node) const {
        assert(hasNode(node));
        return nodeToIndex.at(node);
    }

    const T &getNode(Index index) const {
        assert(index < nodes.size());
        return nodes[index];
    }

    const IndexList &getOutgoingNodes(Index index) const {
        assert(index < nodes.size());
        return outgoingAdjList[index];
    }

    const IndexList &getIncomingNodes(Index index) const {
        assert(index < nodes.size());
        return incomingAdjList[index];
    }

    const IndexList &getOutgoingNodes(const T &node) const {
        assert(hasNode(node));
        return outgoingAdjList[getNodeIndex(node)];
    }

    const IndexList &getIncomingNodes(const T &node) const {
        assert(hasNode(node));
        return incomingAdjList[getNodeIndex(node)];
    }

 private:
    std::vector<T> nodes;
    std::unordered_map<T, Index> nodeToIndex;
    std::vector<std::vector<Index>> outgoingAdjList;
    std::vector<std::vector<Index>> incomingAdjList;

    Index addNodeHelper(const T &node) {
        if (nodeToIndex.find(node) != nodeToIndex.end()) {
            return nodeToIndex[node];
        }
        Index index = nodes.size();
        nodes.push_back(node);
        nodeToIndex[node] = index;
        outgoingAdjList.emplace_back();
        incomingAdjList.emplace_back();
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
