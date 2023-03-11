#include "catch.hpp"
#include "../../TestHelper.h"
#include "commons/graph/Graph.h"

template<typename T>
class TestGraphHelper : public Graph<T> {
 public:
    TestGraphHelper() : Graph<T>() {}

    ~TestGraphHelper() = default;

    const IndexList &getOutgoingNodesPublic(Index index) const {
        return this->getOutgoingNodes(index);
    }

    const IndexList &getIncomingNodesPublic(Index index) const {
        return this->getIncomingNodes(index);
    }

    const IndexList &getOutgoingNodesPublic(const T &node) const {
        return this->getOutgoingNodes(node);
    }

    const IndexList &getIncomingNodesPublic(const T &node) const {
        return this->getIncomingNodes(node);
    }

    bool isNeighborFromPublic(const T &from, const T &to) const {
        if (!this->hasNode(from) || !this->hasNode(to)) {
            return false;
        }
        const IndexList &outgoingNodes = this->getOutgoingNodes(from);
        return std::find(outgoingNodes.begin(), outgoingNodes.end(), this->getNodeIndex(to)) != outgoingNodes.end();
    }

    bool isNeighborToPublic(const T &from, const T &to) const {
        if (!this->hasNode(from) || !this->hasNode(to)) {
            return false;
        }
        const IndexList &incomingNodes = this->getIncomingNodes(from);
        return std::find(incomingNodes.begin(), incomingNodes.end(), this->getNodeIndex(to)) != incomingNodes.end();
    }
};

TEST_CASE("1. test addEdge() method") {
    SECTION("1.1. test line graph") {
        TestGraphHelper<int> g;

        for (int i = 1; i < 10; ++i) {
            g.addEdge(i, i + 1);
        }

        requireFalse(g.isCyclic());
        requireFalse(g.hasNode(0));

        for (int i = 1; i <= 10; ++i) {
            requireTrue(g.hasNode(i));
        }

        requireEqual(g.getNoOfNodes(), 10);

        for (int i = 1; i < 10; ++i) {
            requireTrue(g.isNeighborFromPublic(i, i + 1));
        }

        for (int i = 1; i < 10; ++i) {
            requireFalse(g.isNeighborFromPublic(i + 1, i));
        }

        for (int i = 1; i < 10; ++i) {
            requireTrue(g.isNeighborToPublic(i + 1, i));
        }

        for (int i = 1; i < 10; ++i) {
            requireFalse(g.isNeighborToPublic(i, i + 1));
        }
    }

    SECTION("1.2. fan-in fan-out graph") {
        TestGraphHelper<int> g;

        g.addEdge(1, 2);
        g.addEdge(2, 3);
        g.addEdge(2, 4);
        g.addEdge(2, 5);
        g.addEdge(2, 6);
        g.addEdge(2, 7);
        g.addEdge(2, 8);
        g.addEdge(3, 9);
        g.addEdge(4, 10);
        g.addEdge(5, 11);
        g.addEdge(6, 12);
        g.addEdge(7, 13);
        g.addEdge(8, 14);
        g.addEdge(9, 15);
        g.addEdge(10, 15);
        g.addEdge(11, 15);
        g.addEdge(12, 15);
        g.addEdge(13, 15);
        g.addEdge(14, 15);
        g.addEdge(15, 16);

        requireFalse(g.isCyclic());
        requireFalse(g.hasNode(0));
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(1).size()), 1);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(2).size()), 6);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(3).size()), 1);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(4).size()), 1);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(5).size()), 1);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(6).size()), 1);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(7).size()), 1);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(8).size()), 1);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(9).size()), 1);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(10).size()), 1);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(11).size()), 1);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(12).size()), 1);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(13).size()), 1);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(14).size()), 1);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(15).size()), 1);
        requireEqual(static_cast<int>(g.getIncomingNodesPublic(15).size()), 6);
        requireEqual(static_cast<int>(g.getOutgoingNodesPublic(16).size()), 0);
        requireEqual(static_cast<int>(g.getIncomingNodesPublic(16).size()), 1);
        requireEqual(g.getNoOfNodes(), 16);

        for (int i = 1; i <= 16; ++i) {
            requireTrue(g.hasNode(i));
        }
    }
}

TEST_CASE("2. test isCyclic() method") {
    SECTION("2.1. test line graph") {
        TestGraphHelper<int> g;

        for (int i = 1; i < 10; ++i) {
            g.addEdge(i, i + 1);
        }

        requireFalse(g.isCyclic());
    }

    SECTION("2.2. test line graph loop back") {
        TestGraphHelper<int> g;

        for (int i = 1; i < 10; ++i) {
            g.addEdge(i, i + 1);
        }

        g.addEdge(10, 1);

        requireTrue(g.isCyclic());
    }

    SECTION("2.3. test clique graph size 4") {
        TestGraphHelper<int> g;

        g.addEdge(1, 2);
        g.addEdge(1, 3);
        g.addEdge(1, 4);
        g.addEdge(2, 3);
        g.addEdge(2, 1);
        g.addEdge(2, 4);
        g.addEdge(3, 1);
        g.addEdge(3, 2);
        g.addEdge(3, 4);
        g.addEdge(4, 1);
        g.addEdge(4, 2);
        g.addEdge(4, 3);

        requireTrue(g.isCyclic());
    }

    SECTION("2.4. test tree graph") {
        TestGraphHelper<int> g;

        g.addEdge(1, 2);
        g.addEdge(1, 3);
        g.addEdge(1, 4);
        g.addEdge(2, 5);
        g.addEdge(2, 6);
        g.addEdge(3, 7);
        g.addEdge(3, 8);
        g.addEdge(4, 9);
        g.addEdge(4, 10);

        requireFalse(g.isCyclic());
    }
}

TEST_CASE("3. test == and != operator") {
    SECTION("3.1. test line graph") {
        TestGraphHelper<int> g1;
        TestGraphHelper<int> g2;

        for (int i = 1; i < 10; ++i) {
            g1.addEdge(i, i + 1);
            g2.addEdge(i, i + 1);
        }

        requireTrue(g1 == g2);
        requireFalse(g1 != g2);
    }

    SECTION("3.2. test line graph loop back") {
        TestGraphHelper<int> g1;
        TestGraphHelper<int> g2;

        for (int i = 1; i < 10; ++i) {
            g1.addEdge(i, i + 1);
            g2.addEdge(i, i + 1);
        }

        g1.addEdge(10, 1);
        g2.addEdge(10, 1);

        requireTrue(g1 == g2);
        requireFalse(g1 != g2);
    }

    SECTION("3.3. test clique graph size 4") {
        TestGraphHelper<int> g1;
        TestGraphHelper<int> g2;

        g1.addEdge(1, 2);
        g1.addEdge(1, 3);
        g1.addEdge(1, 4);
        g1.addEdge(2, 3);
        g1.addEdge(2, 1);
        g1.addEdge(2, 4);
        g1.addEdge(3, 1);
        g1.addEdge(3, 2);
        g1.addEdge(3, 4);
        g1.addEdge(4, 1);
        g1.addEdge(4, 2);
        g1.addEdge(4, 3);

        g2.addEdge(1, 2);
        g2.addEdge(1, 3);
        g2.addEdge(1, 4);
        g2.addEdge(2, 3);
        g2.addEdge(2, 1);
        g2.addEdge(2, 4);
        g2.addEdge(3, 1);
        g2.addEdge(3, 2);
        g2.addEdge(3, 4);
        g2.addEdge(4, 1);
        g2.addEdge(4, 2);
        g2.addEdge(4, 3);

        requireTrue(g1 == g2);
        requireFalse(g1 != g2);
    }

    SECTION("3.4. test inequality 1") {
        TestGraphHelper<int> g1;
        TestGraphHelper<int> g2;

        g1.addEdge(1, 2);
        g1.addEdge(1, 3);
        g1.addEdge(1, 4);
        g1.addEdge(2, 3);
        g1.addEdge(2, 1);
        g1.addEdge(2, 4);
        g1.addEdge(3, 1);
        g1.addEdge(3, 2);
        g1.addEdge(3, 4);
        g1.addEdge(4, 1);
        g1.addEdge(4, 2);
        g1.addEdge(4, 3);

        g2.addEdge(1, 2);
        g2.addEdge(1, 3);
        g2.addEdge(1, 4);
        g2.addEdge(2, 3);
        g2.addEdge(2, 1);
        g2.addEdge(2, 4);
        g2.addEdge(3, 1);
        g2.addEdge(3, 2);
        g2.addEdge(3, 4);
        g2.addEdge(4, 1);
        g2.addEdge(4, 2);
        g2.addEdge(4, 3);
        g2.addEdge(5, 6);

        requireFalse(g1 == g2);
        requireTrue(g1 != g2);
    }

    SECTION("3.5. test inequality 2") {
        TestGraphHelper<int> g1;
        TestGraphHelper<int> g2;

        g1.addEdge(1, 2);
        g1.addEdge(1, 3);
        g1.addEdge(1, 4);
        g1.addEdge(2, 3);
        g1.addEdge(2, 1);
        g1.addEdge(2, 4);
        g1.addEdge(3, 1);
        g1.addEdge(3, 2);
        g1.addEdge(3, 4);
        g1.addEdge(4, 1);
        g1.addEdge(4, 2);
        g1.addEdge(4, 3);

        g2.addEdge(1, 2);
        g2.addEdge(1, 3);
        g2.addEdge(1, 4);
        g2.addEdge(2, 3);
        g2.addEdge(2, 1);
        g2.addEdge(2, 4);
        g2.addEdge(3, 1);
        g2.addEdge(3, 2);
        g2.addEdge(3, 4);
        g2.addEdge(4, 1);
        g2.addEdge(4, 2);
        g2.addEdge(4, 3);
        g2.addEdge(5, 6);
        g2.addEdge(5, 7);
        g2.addEdge(5, 8);
        g2.addEdge(6, 7);
        g2.addEdge(6, 5);
        g2.addEdge(6, 8);
        g2.addEdge(7, 5);
        g2.addEdge(7, 6);
        g2.addEdge(7, 8);
        g2.addEdge(8, 5);
        g2.addEdge(8, 6);
        g2.addEdge(8, 7);

        requireFalse(g1 == g2);
        requireTrue(g1 != g2);
    }
}
