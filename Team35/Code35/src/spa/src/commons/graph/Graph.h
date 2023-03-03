#pragma once

#include <cassert>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef uint32_t Index;
typedef std::vector<Index> IndexList;

template<typename T>
class Graph {
 public:
    Graph() : nodes(), nodeToIndex(), outgoingAdjList(), incomingAdjList() {};
    ~Graph() = default;

    bool operator==(const Graph &other) const {
        return nodes == other.nodes && nodeToIndex == other.nodeToIndex &&
            outgoingAdjList == other.outgoingAdjList && incomingAdjList == other.incomingAdjList;
    }

    bool operator!=(const Graph &other) const {
        return !(*this == other);
    }

    void addEdge(const T &from, const T &to) {
        Index fromIndex = addNode(from);
        Index toIndex = addNode(to);
        outgoingAdjList[fromIndex].push_back(toIndex);
        incomingAdjList[toIndex].push_back(fromIndex);
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

    int getNoOfNodes() {
        return nodes.size();
    }

 protected:
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

    Index addNode(const T &node) {
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
