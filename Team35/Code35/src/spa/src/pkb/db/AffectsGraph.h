#pragma once

#include "commons/graph/Graph.h"
#include "commons/types.h"

class AffectsGraph : public Graph<STMT_NUM> {
 public:
    AffectsGraph() = default;

    ~AffectsGraph() override = default;

    void addAffectsEdge(STMT_NUM first, STMT_NUM second);

    bool isEdgeExist(STMT_NUM first, STMT_NUM second);

 private:
    STMT_STMT_SET visited;
};
