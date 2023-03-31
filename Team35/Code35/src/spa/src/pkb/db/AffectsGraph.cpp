#include "AffectsGraph.h"

#include "commons/types.h"

void AffectsGraph::addAffectsEdge(STMT_NUM first, STMT_NUM second, bool isAffects) {
    if (is_affects_map.find(STMT_STMT(first, second)) != is_affects_map.end()) {
        return;
    }
    is_affects_map.emplace(STMT_STMT{first, second}, isAffects);
    if (isAffects) {
        this->addEdge(first, second);
    }
}

bool AffectsGraph::hasAffectsRelationship(STMT_NUM first, STMT_NUM second) {
    return is_affects_map.find(STMT_STMT{first, second}) != is_affects_map.end();
}

bool AffectsGraph::isAffectsRelationshipTrue(STMT_NUM first, STMT_NUM second) {
    if (!hasAffectsRelationship(first, second)) {
        return false;
    }
    return is_affects_map.find(STMT_STMT{first, second})->second;
}

STMT_STMT_SET AffectsGraph::getAllAffectsRelationships() {
    STMT_STMT_SET result;
    for (int i = 0; i < this->getNoOfNodes(); ++i) {
        STMT_NUM stmt1 = this->getNode(i);
        IndexQueue frontier;
        IndexSet visited;
        frontier.push(i);
        while (!frontier.empty()) {
            Index current = frontier.front();
            frontier.pop();
            if (visited.find(current) != visited.end()) {
                continue;
            }
            visited.emplace(current);
            for (Index stmt2_index : this->getOutgoingNodes(current)) {
                STMT_NUM stmt2 = this->getNode(stmt2_index);
                result.emplace(stmt1, stmt2);
                frontier.push(stmt2_index);
            }
        }
    }
    return result;
}
