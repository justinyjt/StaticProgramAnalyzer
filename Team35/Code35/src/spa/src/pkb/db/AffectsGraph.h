#pragma once

#include "commons/graph/Graph.h"
#include "commons/types.h"

class AffectsGraph : public Graph<STMT_NUM> {
 public:
    AffectsGraph() = default;

    ~AffectsGraph() override = default;
};
