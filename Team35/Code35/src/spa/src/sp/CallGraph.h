#pragma once

#include "commons/graph/Graph.h"
#include "commons/types.h"

class CallGraph : public Graph<ENT_NAME> {
 public:
    CallGraph();

    void addTransitiveCallRelationship(const ENT_NAME &caller, const ENT_NAME &callee);

    ENT_ENT_SET getTransitiveCalls();

 private:
    ENT_SET getCallEntities(Index index);
};


