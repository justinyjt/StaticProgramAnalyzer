#include "AffectsGraph.h"

#include "commons/types.h"

void AffectsGraph::addAffectsEdge(STMT_NUM first, STMT_NUM second) {
    if (direct_affects_set_.find(STMT_STMT(first, second)) != direct_affects_set_.end()) {
        return;
    }
    direct_affects_set_.emplace(first, second);
    this->addEdge(first, second);
}

bool AffectsGraph::hasAffectsRelationship(STMT_NUM first, STMT_NUM second) {
    return direct_affects_set_.find(STMT_STMT(first, second)) != direct_affects_set_.end();
}

const STMT_STMT_SET &AffectsGraph::getAllAffectsRelationships(UsageType usageType) {
    if (usageType == UsageType::Direct) {
        return direct_affects_set_;
    }
    if (transitive_affects_set_.has_value()) {
        return transitive_affects_set_.value();
    }
    STMT_STMT_SET result;
    for (int i = 0; i < this->getNoOfNodes(); ++i) {
        this->populateAffectsRelationshipOfIndex(i, result);
    }
    transitive_affects_set_ = result;
    return transitive_affects_set_.value();
}

void AffectsGraph::populateAffectsRelationshipOfIndex(Index idx, STMT_STMT_SET &result) const {
    STMT_NUM stmt1 = this->getNode(idx);
    IndexQueue frontier;
    IndexSet visited;
    frontier.push(idx);
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

void AffectsGraph::reset() {
    Graph<STMT_NUM>::reset();
    direct_affects_set_.clear();
    transitive_affects_set_.reset();
}
