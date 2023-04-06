#include "commons/graph/CallGraph.h"

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
    ENT_ENT_SET result;
    for (int i = 0; i < this->getNoOfNodes(); i++) {
        ENT_NAME caller = this->getNode(i);
        IndexQueue calleeQueue;
        calleeQueue.push(i);
        while (!calleeQueue.empty()) {
            Index current = calleeQueue.front();
            calleeQueue.pop();
            for (Index calleeIndex : this->getOutgoingNodes(current)) {
                ENT_NAME callee = this->getNode(calleeIndex);
                result.emplace(caller, callee);
                calleeQueue.push(calleeIndex);
            }
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
    ENT_ENT_SET result;
    for (int i = 0; i < this->getNoOfNodes(); i++) {
        ENT_NAME caller = this->getNode(i);
        ENT_SET callees = this->getCallEntities(i);
        for (const auto &callee : callees) {
            result.emplace(caller, callee);
        }
    }
    return result;
}

ENT_SET CallGraph::getCallEntities(Index index) {
    ENT_SET callees;
    IndexSet calleeIndices = this->getOutgoingNodes(index);
    for (auto &callee : calleeIndices) {
        callees.emplace(this->getNode(callee));
    }
    return callees;
}

STMT_SET CallGraph::getStmts(const ENT_NAME &procName) const {
    STMT_SET result;
    if (!this->hasNode(procName)) {
        return result;
    }
    IndexQueue calleeQueue;
    Index idx = this->getNodeIndex(procName);
    calleeQueue.push(idx);
    while (!calleeQueue.empty()) {
        Index current = calleeQueue.front();
        calleeQueue.pop();
        STMT_SET currentStmts = this->getStmtsByIndex(current);
        result.insert(currentStmts.begin(), currentStmts.end());
        for (Index calleeIndex : this->getOutgoingNodes(current)) {
            calleeQueue.push(calleeIndex);
        }
    }
    return result;
}

STMT_SET CallGraph::getStmtsByIndex(Index index) const {
    STMT_SET result;
    ENT_NAME proc = this->getNode(index);
    STMT_STMT curr = call_map_.at(proc);
    for (int i = curr.first; i <= curr.second; ++i) {
        result.emplace(i);
    }
    return result;
}

void CallGraph::addProcScope(ENT_NAME proc, STMT_STMT scope) {
    if (!this->hasNode(proc)) {
        return;
    }
    this->call_map_.emplace(proc, scope);
}

ENT_SET CallGraph::getCallees(const ENT_NAME &procName) {
    return this->getCallEntities(getNodeIndex(procName));
}
