#pragma once

#include "CFGraphNodeData.h"
#include "Graph.h"
#include "commons/types.h"

namespace CFG {
class CFGraph : public Graph<CFGraphNodeData> {
 public:
    static CFGraphNodeData end_node_data;
    CFGraph();
};
}  // namespace CFG
