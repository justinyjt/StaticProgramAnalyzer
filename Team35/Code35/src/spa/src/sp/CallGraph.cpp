#include "CallGraph.h"

#include <cassert>

CallGraph::CallGraph() : Graph<ENT_NAME>() {}

/**
 * Add Entities to call graph.
 *
 * @param caller Parent entity invoking callee.
 * @param callee Child entity called by caller.
 */
void CallGraph::addCallRelationship(const ENT_NAME &caller, const ENT_NAME &callee) {
    this->addEdge(caller, callee);
}

/**
 * Get all ENT_ENT pairings for transitive calls.
 *
 * @return All transitive calls
 * i.e. Grandparent calls Parent calls Child
 * returns (Grandparent, Parent), (Grandparent, Child), (Parent, Child)
 */
ENT_ENT_SET CallGraph::getTransitiveCalls() {
    assert(!isCyclic());
    ENT_ENT_SET result;
    for (int i = 0; i < this->getNoOfNodes(); i++) {
        ENT_NAME caller = this->getNode(i);
        IndexQueue calleeQueue;
        IndexList calleeIndices = this->getOutgoingNodes(i);
        calleeQueue.insert(calleeQueue.begin(), calleeIndices.begin(), calleeIndices.end());
        while (calleeQueue.size() > 0) {
            Index current = calleeQueue.front();
            ENT_NAME callee = this->getNode(current);
            result.emplace(caller, callee);
            IndexList indices = this->getOutgoingNodes(current);
            calleeQueue.insert(calleeQueue.end(), indices.begin(), indices.end());
            calleeQueue.pop_front();
        }
    }
    return result;
}

/**
 * Get all ENT_ENT pairings for immediate calls
 * 
 * @return ENT_ENT_SET containing immediate calls
 */
ENT_ENT_SET CallGraph::getImmediateCalls() {
    assert(!isCyclic());
    ENT_ENT_SET result;
    for (int i = 0; i < this->getNoOfNodes(); i++) {
        ENT_NAME caller = this->getNode(i);
        ENT_SET callees = this->getCallEntities(i);
        for (auto callee : callees) {
            result.emplace(caller, callee);
        }
    }
    return result;
}

ENT_SET CallGraph::getCallEntities(Index index) {
    ENT_SET callees;
    IndexList calleeIndices = this->getOutgoingNodes(index);
    for (auto callee : calleeIndices) {
        callees.emplace(this->getNode(callee));
    }
    return callees;
}

