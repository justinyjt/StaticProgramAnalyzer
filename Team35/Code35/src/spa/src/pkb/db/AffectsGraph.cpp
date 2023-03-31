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
