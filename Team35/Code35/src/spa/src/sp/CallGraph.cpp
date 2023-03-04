#include "CallGraph.h"

CallGraph::CallGraph() : Graph<ENT_NAME>() {}

/**
 * Add Entities to call graph.
 *
 * @param caller Parent entity invoking callee.
 * @param callee Child entity called by caller.
 */
void CallGraph::addTransitiveCallRelationship(const ENT_NAME &caller, const ENT_NAME &callee) {
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
    ENT_ENT_SET result;
    for (int i = 0 ; i < this->getNoOfNodes() ; i++) {
        ENT_NAME caller = this->getNode(i);
        ENT_SET callees = this->getCallEntities(i);
        for (auto callee : callees) {
            result.emplace(ENT_ENT(caller, callee));
        }
    }
    return result;
}

ENT_SET CallGraph::getCallEntities(Index index) {
    ENT_SET callees;
    IndexList calleeIndices = this->getOutgoingNodes(index);
    for (auto callee : calleeIndices) {
        callees.emplace(ENT_NAME(this->getNode(callee)));
    }
    return callees;
}

