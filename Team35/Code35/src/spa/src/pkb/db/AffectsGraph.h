#pragma once

#include <unordered_map>

#include "commons/graph/Graph.h"
#include "commons/types.h"

class AffectsGraph : public Graph<STMT_NUM> {
 public:
    AffectsGraph() = default;

    ~AffectsGraph() override = default;

    void addAffectsEdge(STMT_NUM first, STMT_NUM second, bool isAffects = true);

    bool hasAffectsRelationship(STMT_NUM first, STMT_NUM second);

    bool isAffectsRelationshipTrue(STMT_NUM first, STMT_NUM second);

 private:
    std::unordered_map<STMT_STMT, bool, hash_pair> is_affects_map;
};
