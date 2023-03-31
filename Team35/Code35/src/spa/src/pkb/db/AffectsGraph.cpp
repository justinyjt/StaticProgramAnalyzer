#include "AffectsGraph.h"

#include "commons/types.h"

void AffectsGraph::addAffectsEdge(STMT_NUM first, STMT_NUM second) {
    if (visited.find(STMT_STMT(first, second)) != visited.end()) {
        return;
    }
    this->addEdge(first, second);
    visited.emplace(first, second);
}

bool AffectsGraph::isEdgeExist(STMT_NUM first, STMT_NUM second) {
    if (visited.find(STMT_STMT(first, second)) != visited.end()) {
        return true;
    } else if (this->isReachable(first, second, false)) {
        visited.emplace(first, second);
        return true;
    }
    return false;

}
