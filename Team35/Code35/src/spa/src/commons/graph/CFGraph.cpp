#include "CFGraph.h"

#include "CFGraphNodeData.h"

namespace CFG {
CFGraphNodeData CFGraph::end_node_data = makeDummyNodeData(0);

CFGraph::CFGraph() : Graph<CFGraphNodeData>() {}
}  // namespace CFG
